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
static bool ntp_synced = false;
static uint32_t boot_time_seconds = 0;
static uint32_t last_rtc_check = 0;
static const uint32_t RTC_CHECK_INTERVAL = 300; /* 5 minutes in seconds */
static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ipv4_cb;
static size_t wifi_network_idx = 0;

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
			wifi_network_idx++;
		}
	} else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
		LOG_INF("WiFi disconnected");
		wifi_connected = false;
	}
}

static void ipv4_addr_handler(struct net_mgmt_event_callback *cb,
			      uint64_t mgmt_event, struct net_if *iface)
{
	if (mgmt_event == NET_EVENT_IPV4_ADDR_ADD) {
		char buf[16];
		const struct in_addr *addr = net_if_ipv4_get_global_addr(iface,
							NET_ADDR_PREFERRED);
		if (addr) {
			net_addr_ntop(AF_INET, addr, buf, sizeof(buf));
			LOG_INF("IP address obtained: %s", buf);
		} else {
			LOG_INF("IP address assigned");
		}
	}
}

static void start_wifi_connect(void)
{
	struct net_if *iface = net_if_get_default();
	struct wifi_connect_req_params wifi_params = { 0 };

	if (!iface || wifi_connected) {
		return;
	}

	if (wifi_network_idx >= KNOWN_NETWORKS_COUNT) {
		wifi_network_idx = 0;
	}

	const struct wifi_network *net = &known_networks[wifi_network_idx];

	wifi_params.ssid = net->ssid;
	wifi_params.ssid_length = strlen(net->ssid);
	wifi_params.psk = net->password;
	wifi_params.psk_length = strlen(net->password);
	wifi_params.security = WIFI_SECURITY_TYPE_PSK;
	wifi_params.channel = WIFI_CHANNEL_ANY;

	LOG_INF("Attempting to connect to WiFi: %s", net->ssid);
	int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params,
			 sizeof(struct wifi_connect_req_params));
	if (ret != 0) {
		LOG_ERR("WiFi connect request failed: %d", ret);
		wifi_network_idx++;
	}
}

/* Try a single NTP sync attempt (non-blocking-friendly, short timeout) */
static int try_ntp_sync(void)
{
	struct sntp_time sntp_time;
	int ret;

	LOG_INF("Trying NTP sync...");
	ret = sntp_simple("time.cloudflare.com", 2000, &sntp_time);
	if (ret < 0) {
		LOG_WRN("NTP failed: %d", ret);
		return ret;
	}

	/* Apply Europe/Berlin timezone offset (CET=UTC+1, CEST=UTC+2) */
	time_t ts = (time_t)sntp_time.seconds;
	struct tm *utc = gmtime(&ts);
	if (utc == NULL) {
		return -1;
	}

	/* DST: last Sunday of March 02:00 UTC to last Sunday of October 03:00 UTC */
	bool is_dst = false;
	if (utc->tm_mon > 2 && utc->tm_mon < 9) {
		/* April through September: always CEST */
		is_dst = true;
	} else if (utc->tm_mon == 2) {
		/* March: CEST after last Sunday at 02:00 UTC */
		int last_sun = 31 - ((5 + utc->tm_year + utc->tm_year / 4) % 7);
		if (utc->tm_mday > last_sun ||
		    (utc->tm_mday == last_sun && utc->tm_hour >= 2)) {
			is_dst = true;
		}
	} else if (utc->tm_mon == 9) {
		/* October: CET after last Sunday at 03:00 UTC */
		int last_sun = 31 - ((2 + utc->tm_year + utc->tm_year / 4) % 7);
		if (utc->tm_mday < last_sun ||
		    (utc->tm_mday == last_sun && utc->tm_hour < 3)) {
			is_dst = true;
		}
	}

	ts += is_dst ? 7200 : 3600;
	struct tm *local = gmtime(&ts);
	if (local == NULL) {
		return -1;
	}

	LOG_INF("Timezone: %s (UTC%s)", is_dst ? "CEST" : "CET",
		is_dst ? "+2" : "+1");

	struct rtc_time rtc_time = {
		.tm_year = local->tm_year,
		.tm_mon = local->tm_mon,
		.tm_mday = local->tm_mday,
		.tm_hour = local->tm_hour,
		.tm_min = local->tm_min,
		.tm_sec = local->tm_sec,
		.tm_wday = local->tm_wday,
	};

	if (device_is_ready(rtc)) {
		ret = rtc_set_time(rtc, &rtc_time);
		if (ret == 0) {
			LOG_INF("Online time: %04d-%02d-%02d %02d:%02d:%02d",
				rtc_time.tm_year + 1900, rtc_time.tm_mon + 1,
				rtc_time.tm_mday, rtc_time.tm_hour,
				rtc_time.tm_min, rtc_time.tm_sec);
			/* Read back RTC to confirm */
			struct rtc_time readback = { 0 };
			if (rtc_get_time(rtc, &readback) == 0) {
				LOG_INF("Device time: %04d-%02d-%02d %02d:%02d:%02d",
					readback.tm_year + 1900,
					readback.tm_mon + 1,
					readback.tm_mday, readback.tm_hour,
					readback.tm_min, readback.tm_sec);
			}
			return 0;
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

static void disconnect_wifi(void)
{
	struct net_if *iface = net_if_get_default();

	if (!iface || !wifi_connected) {
		return;
	}

	int ret = net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0);
	if (ret != 0) {
		LOG_ERR("WiFi disconnect failed: %d", ret);
	}
	wifi_connected = false;
}

/* WiFi/NTP state machine states */
enum sync_state {
	SYNC_IDLE,
	SYNC_CONNECTING,
	SYNC_NTP,
	SYNC_DONE,
};

static enum sync_state sync_state = SYNC_IDLE;
static uint32_t sync_timer = 0;
static int ntp_attempts = 0;

static void sync_start(void)
{
	sync_state = SYNC_CONNECTING;
	sync_timer = 0;
	ntp_attempts = 0;
	wifi_network_idx = 0;
	start_wifi_connect();
}

/* Called once per second from main loop. Returns true when sync complete. */
static bool sync_tick(void)
{
	sync_timer++;

	switch (sync_state) {
	case SYNC_IDLE:
		break;

	case SYNC_CONNECTING:
		if (wifi_connected) {
			/* Wait a few seconds for DHCP before first NTP attempt */
			if (sync_timer >= 5) {
				LOG_INF("Trying NTP...");
				sync_state = SYNC_NTP;
				sync_timer = 0;
			}
		} else if (sync_timer >= 10) {
			wifi_network_idx++;
			if (wifi_network_idx >= KNOWN_NETWORKS_COUNT * 3) {
				LOG_WRN("WiFi connect failed after trying all networks");
				sync_state = SYNC_DONE;
				return true;
			}
			start_wifi_connect();
			sync_timer = 0;
		}
		break;

	case SYNC_NTP:
		/* Try NTP every 3 seconds */
		if (sync_timer >= 3) {
			sync_timer = 0;
			if (try_ntp_sync() == 0) {
				ntp_synced = true;
				disconnect_wifi();
				sync_state = SYNC_DONE;
				return true;
			}
			ntp_attempts++;
			if (ntp_attempts >= 10) {
				LOG_WRN("NTP failed after %d attempts, giving up",
					ntp_attempts);
				disconnect_wifi();
				sync_state = SYNC_DONE;
				return true;
			}
		}
		break;

	case SYNC_DONE:
		break;
	}

	return false;
}

int main(void)
{
	char counter_buf[32] = { 0 };
	const struct device *display;
	lv_obj_t *counter_label;
	lv_obj_t *wifi_label;
	lv_style_t counter_label_style;
	lv_style_t wifi_label_style;
	bool rtc_has_time = false;
	int ret;

	LOG_INF("main() started");

	/* Initialize display */
	display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (display == NULL || !device_is_ready(display)) {
		LOG_ERR("display not ready");
		return 0;
	}

	/* Check if RTC already has valid time */
	if (device_is_ready(rtc)) {
		struct rtc_time rtc_check = { 0 };
		if (rtc_get_time(rtc, &rtc_check) == 0) {
			time_t now_epoch = rtc_time_to_epoch(&rtc_check);
			time_t start_epoch = rtc_time_to_epoch(&START_TIME);
			if (now_epoch > start_epoch) {
				LOG_INF("RTC has valid time: %02d:%02d:%02d",
					rtc_check.tm_hour, rtc_check.tm_min,
					rtc_check.tm_sec);
				rtc_has_time = true;
			}
		}
	}

	/* Create UI - initially hidden */
	lv_style_init(&counter_label_style);
	lv_style_set_text_font(&counter_label_style, &lv_font_montserrat_14);

	counter_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(counter_label, &counter_label_style, 0);
	lv_obj_align(counter_label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(counter_label, "");

	lv_style_init(&wifi_label_style);
	lv_style_set_text_font(&wifi_label_style, &lv_font_montserrat_10);

	wifi_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(wifi_label, &wifi_label_style, 0);
	lv_obj_align(wifi_label, LV_ALIGN_BOTTOM_RIGHT, -2, -3);
	lv_label_set_text(wifi_label, "");

	lv_timer_handler();
	ret = display_blanking_off(display);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("display blanking off failed (%d)", ret);
		return 0;
	}

	/* Register WiFi callback and kick off initial sync */
	net_mgmt_init_event_callback(&wifi_cb, wifi_connect_handler,
				     NET_EVENT_WIFI_CONNECT_RESULT |
				     NET_EVENT_WIFI_DISCONNECT_RESULT);
	net_mgmt_add_event_callback(&wifi_cb);

	net_mgmt_init_event_callback(&ipv4_cb, ipv4_addr_handler,
				     NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&ipv4_cb);
	sync_start();

	uint32_t last_elapsed = UINT32_MAX;

	while (1) {
		/* Update display only when value changes (avoids redundant SPI writes) */
		if (rtc_has_time) {
			uint32_t elapsed = calculate_elapsed_seconds();
			if (elapsed != last_elapsed) {
				last_elapsed = elapsed;
				snprintf(counter_buf, sizeof(counter_buf), "%u",
					 elapsed);
				lv_label_set_text(counter_label, counter_buf);
			}
		}

		/* Drive the WiFi/NTP state machine */
		if (sync_state != SYNC_IDLE && sync_state != SYNC_DONE) {
			lv_label_set_text(wifi_label, LV_SYMBOL_WIFI);
			if (sync_tick()) {
				lv_label_set_text(wifi_label, "");
				if (ntp_synced) {
					rtc_has_time = true;
				}
			}
		}

		/* Periodic re-sync every 5 minutes */
		if (sync_state == SYNC_DONE &&
		    (boot_time_seconds % RTC_CHECK_INTERVAL) == 0 &&
		    boot_time_seconds != last_rtc_check) {
			last_rtc_check = boot_time_seconds;
			sync_start();
		}

		lv_timer_handler();
		k_sleep(K_SECONDS(1));
		boot_time_seconds++;
	}
}
