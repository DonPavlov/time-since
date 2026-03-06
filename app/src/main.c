/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h> /* for abs() */
#include <time.h>

#include <lvgl.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/sntp.h>

#include "start_time.h"
#include "wifi_creds.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));

/* Global state */
static bool wifi_connected = false;
static uint32_t boot_time_seconds = 0;
static uint32_t last_rtc_check = 0;
static const uint32_t RTC_CHECK_INTERVAL = 300; /* 5 minutes in seconds */

static void wifi_connect_handler(struct net_mgmt_event_callback *cb,
				uint64_t mgmt_event, struct net_if *iface)
{
	if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
		const struct wifi_status *status = (const struct wifi_status *)cb->info;
		if (status->status == 0) {
			LOG_INF("WiFi connected successfully");
			wifi_connected = true;
		} else {
			LOG_ERR("WiFi connection failed: %d", status->status);
			wifi_connected = false;
		}
	}
}

static int connect_to_wifi(void)
{
	struct net_if *iface = net_if_get_default();
	struct wifi_connect_req_params wifi_params = { 0 };
	struct net_mgmt_event_callback wifi_cb;

	if (!iface) {
		LOG_ERR("No network interface found");
		return -1;
	}

	/* Try to connect to known networks */
	for (size_t i = 0; i < KNOWN_NETWORKS_COUNT; i++) {
		const struct wifi_network *net = &known_networks[i];

		wifi_params.ssid = net->ssid;
		wifi_params.ssid_length = strlen(net->ssid);
		wifi_params.psk = net->password;
		wifi_params.psk_length = strlen(net->password);
		wifi_params.security = WIFI_SECURITY_TYPE_PSK;
		wifi_params.channel = WIFI_CHANNEL_ANY;

		net_mgmt_init_event_callback(&wifi_cb, wifi_connect_handler,
					   NET_EVENT_WIFI_CONNECT_RESULT);
		net_mgmt_add_event_callback(&wifi_cb);

		LOG_INF("Attempting to connect to WiFi: %s", net->ssid);
		int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params,
				 sizeof(struct wifi_connect_req_params));
		if (ret == 0) {
			/* Wait a bit for connection */
			k_sleep(K_SECONDS(5));
			if (wifi_connected) {
				return 0;
			}
		}
		net_mgmt_del_event_callback(&wifi_cb);
	}

	return -1;
}

static int sync_time_via_ntp(void)
{
	struct sntp_time sntp_time;
	int ret;

	LOG_INF("Syncing time via NTP...");
	ret = sntp_simple("pool.ntp.org", 5000, &sntp_time);
	if (ret < 0) {
		LOG_ERR("NTP sync failed: %d", ret);
		return ret;
	}

	/* Convert SNTP seconds to broken-down time */
	time_t ts = (time_t)sntp_time.seconds;
	struct tm *gm = gmtime(&ts);
	if (gm == NULL) {
		LOG_ERR("gmtime failed");
		return -1;
	}

	struct rtc_time rtc_time = {
		.tm_year = gm->tm_year,
		.tm_mon = gm->tm_mon,
		.tm_mday = gm->tm_mday,
		.tm_hour = gm->tm_hour,
		.tm_min = gm->tm_min,
		.tm_sec = gm->tm_sec,
		.tm_wday = gm->tm_wday,
	};

	if (device_is_ready(rtc)) {
		ret = rtc_set_time(rtc, &rtc_time);
		if (ret == 0) {
			LOG_INF("RTC updated via NTP: %04d-%02d-%02d %02d:%02d:%02d",
				rtc_time.tm_year + 1900, rtc_time.tm_mon + 1, rtc_time.tm_mday,
				rtc_time.tm_hour, rtc_time.tm_min, rtc_time.tm_sec);
			return 0;
		} else {
			LOG_ERR("Failed to set RTC time: %d", ret);
		}
	}

	return ret;
}

static time_t rtc_time_to_epoch(const struct rtc_time *t)
{
	struct tm tm_val = {
		.tm_year = t->tm_year,
		.tm_mon = t->tm_mon,
		.tm_mday = t->tm_mday,
		.tm_hour = t->tm_hour,
		.tm_min = t->tm_min,
		.tm_sec = t->tm_sec,
	};
	return mktime(&tm_val);
}

static uint32_t calculate_elapsed_seconds(void)
{
	struct rtc_time now = { 0 };

	if (device_is_ready(rtc) && rtc_get_time(rtc, &now) == 0) {
		time_t now_epoch = rtc_time_to_epoch(&now);
		time_t start_epoch = rtc_time_to_epoch(&START_TIME);

		if (now_epoch > start_epoch) {
			return (uint32_t)(now_epoch - start_epoch);
		}
		return 0;
	}

	/* If RTC not available, use boot time counter */
	return boot_time_seconds;
}

static void check_and_sync_rtc(void)
{
	struct rtc_time rtc_now = { 0 };
	uint32_t rtc_seconds = 0;
	uint32_t local_seconds = 0;

	if (!device_is_ready(rtc) || !wifi_connected) {
		return;
	}

	/* Get current RTC time */
	if (rtc_get_time(rtc, &rtc_now) != 0) {
		return;
	}

	rtc_seconds = (uint32_t)(rtc_now.tm_hour * 3600 +
				rtc_now.tm_min * 60 +
				rtc_now.tm_sec);

	/* Get NTP time for comparison */
	struct sntp_time sntp_time;
	if (sntp_simple("pool.ntp.org", 2000, &sntp_time) == 0) {
		local_seconds = sntp_time.seconds % 86400; /* Seconds since midnight */

		/* Check if difference is > 5 seconds */
		int32_t diff = (int32_t)rtc_seconds - (int32_t)local_seconds;
		if (abs(diff) > 5) {
			LOG_INF("RTC time drift detected (%d seconds), syncing...", diff);
			sync_time_via_ntp();
		}
	}
}

int main(void)
{
	char counter_buf[32] = { 0 };
	const struct device *display;
	struct display_capabilities caps;
	lv_obj_t *counter_label;
	lv_obj_t *wifi_label;
	lv_style_t counter_label_style;
	int ret;

	LOG_INF("main() started");

	/* Initialize display */
	LOG_INF("Getting display device...");
	display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (display == NULL) {
		LOG_ERR("display device is NULL");
		return 0;
	}

	LOG_INF("Checking if display is ready...");
	if (!device_is_ready(display)) {
		LOG_ERR("display not ready");
		return 0;
	}
	LOG_INF("Display is ready!");

	display_get_capabilities(display, &caps);
	LOG_INF("Display caps: x_res=%d, y_res=%d", caps.x_resolution, caps.y_resolution);

	/* Check if RTC has valid time, if not start counting from 0 */
	if (device_is_ready(rtc)) {
		struct rtc_time rtc_check = { 0 };
		if (rtc_get_time(rtc, &rtc_check) != 0) {
			LOG_INF("RTC not initialized, will start counting from 0");
		} else {
			LOG_INF("RTC has valid time: %02d:%02d:%02d",
				rtc_check.tm_hour, rtc_check.tm_min, rtc_check.tm_sec);
		}
	}

	/* Create UI */
	lv_style_init(&counter_label_style);
	lv_style_set_text_font(&counter_label_style, &lv_font_montserrat_20);

	counter_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(counter_label, &counter_label_style, 0);
	lv_obj_align(counter_label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(counter_label, "0");

	wifi_label = lv_label_create(lv_screen_active());
	lv_obj_align(wifi_label, LV_ALIGN_BOTTOM_RIGHT, -2, -3);
	lv_label_set_text(wifi_label, "");

	lv_timer_handler();
	ret = display_blanking_off(display);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("display blanking off failed (%d)", ret);
		return 0;
	}

	/* Wait 2 seconds before trying WiFi */
	LOG_INF("Waiting 2 seconds before WiFi connection attempt...");
	k_sleep(K_SECONDS(2));

	/* Try to connect to WiFi */
	if (connect_to_wifi() == 0) {
		lv_label_set_text(wifi_label, LV_SYMBOL_WIFI);
		/* Wait for DHCP/DNS to become available */
		k_sleep(K_SECONDS(5));
		/* Sync time via NTP */
		sync_time_via_ntp();
	} else {
		lv_label_set_text(wifi_label, "");
	}

	while (1) {
		/* Update display every second */
		uint32_t elapsed = calculate_elapsed_seconds();
		snprintf(counter_buf, sizeof(counter_buf), "%u", elapsed);
		lv_label_set_text(counter_label, counter_buf);

		/* Check RTC sync every 5 minutes */
		if ((boot_time_seconds % RTC_CHECK_INTERVAL) == 0 &&
		    boot_time_seconds != last_rtc_check) {
			check_and_sync_rtc();
			last_rtc_check = boot_time_seconds;
		}

		lv_timer_handler();
		k_sleep(K_SECONDS(1)); /* Sleep 1 second */

		boot_time_seconds++;
	}
}
