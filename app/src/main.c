/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdbool.h>
#include <time.h>

#include <lvgl.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/pm/device.h>

#include "gui.h"
#include "http_log_server.h"
#include "power.h"
#include "time_utils.h"
#include "wifi.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));
static const struct gpio_dt_spec sleep_btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

static const uint32_t AUTO_SLEEP_SECONDS = 180;
static const uint32_t WIFI_PER_NET_TIMEOUT_MS = 8000;
static const int NTP_MAX_ATTEMPTS = 3;

static struct gpio_callback sleep_btn_cb_data;
static volatile bool sleep_requested;

static time_t base_epoch;
static int64_t base_uptime_ms;

static void sleep_btn_pressed(const struct device *dev, struct gpio_callback *cb,
			      uint32_t pins)
{
	sleep_requested = true;
}

static void capture_base(time_t epoch)
{
	base_epoch = epoch;
	base_uptime_ms = k_uptime_get();
}

static bool read_rtc_epoch(time_t *out)
{
	struct rtc_time rt = { 0 };

	if (!device_is_ready(rtc) || rtc_get_time(rtc, &rt) != 0) {
		return false;
	}

	*out = time_utils_rtc_to_epoch_utc(&rt);
	return true;
}

static uint32_t elapsed_seconds(void)
{
	time_t start = time_utils_start_epoch_utc();

	if (base_epoch == 0 || base_epoch <= start) {
		return 0;
	}

	int64_t delta_ms = k_uptime_get() - base_uptime_ms;
	return (uint32_t)((base_epoch - start) + delta_ms / 1000);
}

static void enter_sleep(const struct device *display)
{
	LOG_INF("Sleep");
	wifi_disconnect();
	display_blanking_on(display);
	(void)pm_device_action_run(display, PM_DEVICE_ACTION_SUSPEND);
	k_sleep(K_MSEC(50));
	(void)power_enter_deep_sleep(&sleep_btn);
}

int main(void)
{
	const struct device *display;
	struct gui_ctx gui;
	time_t epoch;
	bool rtc_has_time = false;

	LOG_INF("main() started");

	display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (display == NULL || !device_is_ready(display)) {
		LOG_ERR("display not ready");
		return 0;
	}

	if (read_rtc_epoch(&epoch) && epoch > time_utils_start_epoch_utc()) {
		capture_base(epoch);
		rtc_has_time = true;
		LOG_INF("RTC valid, base epoch=%lld", (long long)epoch);
	}

	gui_init(&gui, rtc_has_time, elapsed_seconds());
	lv_timer_handler();

	int ret = display_blanking_off(display);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("display blanking off failed (%d)", ret);
		return 0;
	}

	if (gpio_is_ready_dt(&sleep_btn)) {
		gpio_pin_configure_dt(&sleep_btn, GPIO_INPUT);
		gpio_init_callback(&sleep_btn_cb_data, sleep_btn_pressed,
				   BIT(sleep_btn.pin));
		gpio_add_callback(sleep_btn.port, &sleep_btn_cb_data);
		gpio_pin_interrupt_configure_dt(&sleep_btn, GPIO_INT_EDGE_TO_ACTIVE);
	} else {
		LOG_ERR("Sleep button GPIO not ready");
	}

	wifi_module_init();

	if (wifi_connect_any(WIFI_PER_NET_TIMEOUT_MS) == 0) {
		gui_set_wifi_active(&gui, true);
		(void)http_log_server_start();

		if (wifi_sync_ntp(NTP_MAX_ATTEMPTS) == 0) {
			if (read_rtc_epoch(&epoch)) {
				capture_base(epoch);
				rtc_has_time = true;
				gui_set_counter(&gui, elapsed_seconds());
			}
		}
	}

	uint32_t boot_seconds = 0;
	while (1) {
		if (sleep_requested || boot_seconds >= AUTO_SLEEP_SECONDS) {
			enter_sleep(display);
		}

		gui_set_counter(&gui, rtc_has_time ? elapsed_seconds() : boot_seconds);
		gui_set_wifi_active(&gui, wifi_is_connected());
		lv_timer_handler();
		k_sleep(K_SECONDS(1));
		boot_seconds++;
	}
}
