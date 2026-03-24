/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <lvgl.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "gui.h"
#include "power.h"
#include "time_utils.h"
#include "wifi.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));

static const uint32_t RTC_CHECK_INTERVAL = 300;
static const uint32_t AUTO_SLEEP_SECONDS = 180;

/* Sleep button on GPIO0 (D0/A0 on XIAO ESP32-C6) */
static const struct gpio_dt_spec sleep_btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static struct gpio_callback sleep_btn_cb_data;
static volatile bool sleep_requested;
static bool sleep_btn_armed;
static bool sleep_btn_ready;

static uint32_t boot_time_seconds;
static uint32_t last_rtc_check;

static void sleep_btn_pressed(const struct device *dev, struct gpio_callback *cb,
			      uint32_t pins)
{
	sleep_requested = true;
}

static void arm_sleep_button_if_released(void)
{
	if (!sleep_btn_ready || sleep_btn_armed) {
		return;
	}

	int pin_val = gpio_pin_get_dt(&sleep_btn);
	if (pin_val < 0) {
		return;
	}

	if (pin_val != 0) {
		k_sleep(K_MSEC(50));
		if (gpio_pin_get_dt(&sleep_btn) != 0) {
			gpio_pin_interrupt_configure_dt(&sleep_btn,
							GPIO_INT_EDGE_TO_ACTIVE);
			sleep_btn_armed = true;
		}
	}
}

static void enter_sleep(const struct device *display)
{
	LOG_INF("Entering sleep...");

	display_blanking_on(display);
	wifi_disconnect();

	if (!sleep_btn_ready) {
		return;
	}

	if (sleep_btn_armed) {
		gpio_pin_interrupt_configure_dt(&sleep_btn, GPIO_INT_DISABLE);
	}

	while (!gpio_pin_get_dt(&sleep_btn)) {
		k_sleep(K_MSEC(50));
	}

	k_sleep(K_MSEC(80));
	(void)power_enter_deep_sleep(&sleep_btn);
}

static bool rtc_has_valid_time(void)
{
	if (!device_is_ready(rtc)) {
		return false;
	}

	struct rtc_time rtc_check = { 0 };
	time_t now_epoch;
	time_t start_epoch = time_utils_start_epoch_utc();

	if (rtc_get_time(rtc, &rtc_check) != 0) {
		return false;
	}

	now_epoch = time_utils_rtc_to_epoch_utc(&rtc_check);
	if (now_epoch <= start_epoch) {
		return false;
	}

	LOG_INF("RTC has valid time: %02d:%02d:%02d",
		rtc_check.tm_hour, rtc_check.tm_min, rtc_check.tm_sec);
	return true;
}

static uint32_t calculate_elapsed_seconds(bool use_rtc)
{
	struct rtc_time now = { 0 };
	time_t now_epoch;
	time_t start_epoch = time_utils_start_epoch_utc();

	if (use_rtc && device_is_ready(rtc) && rtc_get_time(rtc, &now) == 0) {
		now_epoch = time_utils_rtc_to_epoch_utc(&now);

		if (now_epoch > start_epoch) {
			return (uint32_t)(now_epoch - start_epoch);
		}
		return 0;
	}

	return boot_time_seconds;
}

int main(void)
{
	const struct device *display;
	struct gui_ctx gui;
	bool rtc_has_time = false;
	int ret;

	LOG_INF("main() started");

	display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (display == NULL || !device_is_ready(display)) {
		LOG_ERR("display not ready");
		return 0;
	}

	rtc_has_time = rtc_has_valid_time();

	gui_init(&gui, rtc_has_time,
		 calculate_elapsed_seconds(rtc_has_time));
	lv_timer_handler();

	ret = display_blanking_off(display);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("display blanking off failed (%d)", ret);
		return 0;
	}

	if (!gpio_is_ready_dt(&sleep_btn)) {
		LOG_ERR("Sleep button GPIO not ready");
	} else {
		gpio_pin_configure_dt(&sleep_btn, GPIO_INPUT);
		gpio_init_callback(&sleep_btn_cb_data, sleep_btn_pressed,
				   BIT(sleep_btn.pin));
		gpio_add_callback(sleep_btn.port, &sleep_btn_cb_data);
		sleep_btn_armed = false;
		sleep_btn_ready = true;
	}

	power_init();

	wifi_module_init();
	gui_set_wifi_active(&gui, true);
	wifi_sync_start();

	while (1) {
		bool usb_present = power_usb_present();

		arm_sleep_button_if_released();

		if (sleep_requested) {
			if (usb_present) {
				sleep_requested = false;
			} else {
				k_sleep(K_MSEC(50));
				enter_sleep(display);
			}
		}

		if (!usb_present && boot_time_seconds >= AUTO_SLEEP_SECONDS) {
			enter_sleep(display);
		}

		gui_set_counter(&gui, calculate_elapsed_seconds(rtc_has_time));

		if (wifi_sync_in_progress()) {
			if (wifi_sync_tick()) {
				gui_set_wifi_active(&gui, false);
				if (wifi_rtc_updated()) {
					rtc_has_time = true;
				}
			}
		}

		if (rtc_has_time) {
			gui_set_counter(&gui, calculate_elapsed_seconds(true));
		}

		if (wifi_sync_done() &&
		    (boot_time_seconds % RTC_CHECK_INTERVAL) == 0 &&
		    boot_time_seconds != last_rtc_check) {
			last_rtc_check = boot_time_seconds;
			gui_set_wifi_active(&gui, true);
			wifi_sync_start();
		}

		lv_timer_handler();
		k_sleep(K_SECONDS(1));
		boot_time_seconds++;
	}
}
