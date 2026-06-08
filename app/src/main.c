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
#include "power.h"
#include "time_utils.h"
#include "wifi.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));
static const struct gpio_dt_spec sleep_btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

static const uint32_t AUTO_SLEEP_SECONDS = 180;
static const uint32_t BTN_WAKE_GRACE_SECONDS = 3;
static const int64_t BTN_RELEASE_STABLE_MS = 50;
static const int64_t BTN_RELEASE_POLL_MS = 10;
static const uint32_t WIFI_PER_NET_TIMEOUT_MS = 8000;
static const int NTP_MAX_ATTEMPTS = 3;
static const uint32_t NTP_RETRY_DELAY_SECONDS = 5;
static const uint32_t WIFI_SYNC_ICON_MIN_SECONDS = 3;
static const int64_t WIFI_SYNC_ICON_BLINK_MS = 300;
static const int64_t GUI_TICK_MS = 100;

static struct gpio_callback sleep_btn_cb_data;
static volatile bool sleep_requested;
static volatile bool time_sync_blink;

static time_t base_epoch;
static int64_t base_uptime_ms;
static K_MUTEX_DEFINE(time_base_lock);
static K_THREAD_STACK_DEFINE(time_sync_stack, 6144);
static struct k_thread time_sync_thread_data;

static void sleep_btn_pressed(const struct device *dev, struct gpio_callback *cb,
			      uint32_t pins)
{
	sleep_requested = true;
}

static void capture_base(time_t epoch)
{
	k_mutex_lock(&time_base_lock, K_FOREVER);
	base_epoch = epoch;
	base_uptime_ms = k_uptime_get();
	k_mutex_unlock(&time_base_lock);
}

static time_t current_epoch(void)
{
	time_t epoch;
	int64_t uptime_ms;
	int64_t delta_ms;

	k_mutex_lock(&time_base_lock, K_FOREVER);
	epoch = base_epoch;
	uptime_ms = base_uptime_ms;
	k_mutex_unlock(&time_base_lock);

	if (epoch == 0) {
		return 0;
	}

	delta_ms = k_uptime_get() - uptime_ms;
	return epoch + delta_ms / 1000;
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
	time_t epoch = current_epoch();

	if (epoch == 0 || epoch <= start) {
		return 0;
	}

	return (uint32_t)(epoch - start);
}

static void ntp_retry_started(int ret, void *user_data)
{
	ARG_UNUSED(ret);
	ARG_UNUSED(user_data);

	time_sync_blink = true;
}

static bool wifi_icon_active(int64_t sync_icon_min_until_ms)
{
	bool active = wifi_is_connected() || k_uptime_get() < sync_icon_min_until_ms;

	if (active && time_sync_blink) {
		return ((k_uptime_get() / WIFI_SYNC_ICON_BLINK_MS) % 2) == 0;
	}

	return active;
}

static void time_sync_thread(void *arg1, void *arg2, void *arg3)
{
	time_t before_sync;
	time_t synced_epoch;
	int ret;

	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	LOG_INF("Time sync thread started");

	if (wifi_connect_any(WIFI_PER_NET_TIMEOUT_MS) != 0) {
		time_sync_blink = false;
		return;
	}

	before_sync = current_epoch();
	ret = wifi_sync_ntp(NTP_MAX_ATTEMPTS, NTP_RETRY_DELAY_SECONDS,
			    ntp_retry_started, NULL);
	if (ret != 0) {
		goto out;
	}

	if (!read_rtc_epoch(&synced_epoch)) {
		goto out;
	}

	capture_base(synced_epoch);

	if (before_sync != 0) {
		LOG_INF("NTP adjusted elapsed base by %lld seconds",
			(long long)(synced_epoch - before_sync));
	}

out:
	wifi_disconnect();
	time_sync_blink = false;
}

/*
 * Block until the sleep button has been released (logical inactive) continuously
 * for BTN_RELEASE_STABLE_MS. Deep-sleep wake is level-triggered on GPIO LOW, so
 * arming it while the button is still held would wake the chip immediately. A
 * no-op if the GPIO is not ready, so it can never hang.
 */
static void wait_button_released(void)
{
	int64_t released_since_ms = -1;

	if (!gpio_is_ready_dt(&sleep_btn)) {
		return;
	}

	while (true) {
		if (gpio_pin_get_dt(&sleep_btn) == 0) {
			if (released_since_ms < 0) {
				released_since_ms = k_uptime_get();
			} else if (k_uptime_get() - released_since_ms >=
				   BTN_RELEASE_STABLE_MS) {
				return;
			}
		} else {
			released_since_ms = -1;
		}

		k_sleep(K_MSEC(BTN_RELEASE_POLL_MS));
	}
}

static void enter_sleep(const struct device *display)
{
	LOG_INF("Sleep");
	wifi_disconnect();
	display_blanking_on(display);
	(void)pm_device_action_run(display, PM_DEVICE_ACTION_SUSPEND);
	wait_button_released();
	(void)power_enter_deep_sleep(&sleep_btn);
}

int main(void)
{
	const struct device *display;
	struct gui_ctx gui;
	time_t epoch;
	int64_t boot_uptime_ms;
	int64_t wifi_sync_icon_min_until_ms;

	LOG_INF("main() started");

	display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (display == NULL || !device_is_ready(display)) {
		LOG_ERR("display not ready");
		return 0;
	}

	if (read_rtc_epoch(&epoch) && epoch > time_utils_start_epoch_utc()) {
		capture_base(epoch);
		LOG_INF("RTC valid, base epoch=%lld", (long long)epoch);
	}

	gui_init(&gui, elapsed_seconds());
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
	time_sync_blink = false;
	boot_uptime_ms = k_uptime_get();
	wifi_sync_icon_min_until_ms = boot_uptime_ms +
				      WIFI_SYNC_ICON_MIN_SECONDS * MSEC_PER_SEC;

	k_thread_create(&time_sync_thread_data, time_sync_stack,
			K_THREAD_STACK_SIZEOF(time_sync_stack),
			time_sync_thread, NULL, NULL, NULL,
			K_PRIO_PREEMPT(8), 0, K_NO_WAIT);
	k_thread_name_set(&time_sync_thread_data, "time_sync");

	while (1) {
		uint32_t boot_seconds =
			(uint32_t)((k_uptime_get() - boot_uptime_ms) / MSEC_PER_SEC);
		bool may_sleep = boot_seconds >= BTN_WAKE_GRACE_SECONDS;

		/*
		 * Discard any button press during the boot grace window: the wake
		 * press (and any hold-to-boot) lands here and must not shut the
		 * device back down. Shutdown needs a fresh press after the grace.
		 */
		if (!may_sleep) {
			sleep_requested = false;
		} else if (sleep_requested || boot_seconds >= AUTO_SLEEP_SECONDS) {
			enter_sleep(display);
		}

		gui_set_counter(&gui, elapsed_seconds());
		gui_set_wifi_active(&gui,
				    wifi_icon_active(wifi_sync_icon_min_until_ms));
		lv_timer_handler();
		k_sleep(K_MSEC(GUI_TICK_MS));
	}
}
