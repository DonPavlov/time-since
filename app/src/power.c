/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "power.h"

#include <errno.h>

#include <esp_err.h>
#include <esp_sleep.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/poweroff.h>

LOG_MODULE_REGISTER(power, LOG_LEVEL_DBG);

#if DT_HAS_ALIAS(usb_vbus) && DT_NODE_HAS_STATUS(DT_ALIAS(usb_vbus), okay)
static const struct gpio_dt_spec usb_vbus = GPIO_DT_SPEC_GET(DT_ALIAS(usb_vbus), gpios);

int power_init(void)
{
	if (!device_is_ready(usb_vbus.port)) {
		LOG_WRN("USB VBUS GPIO not ready");
		return -ENODEV;
	}

	return gpio_pin_configure_dt(&usb_vbus, GPIO_INPUT);
}

bool power_usb_present(void)
{
	if (!device_is_ready(usb_vbus.port)) {
		return false;
	}

	int val = gpio_pin_get_dt(&usb_vbus);
	if (val < 0) {
		return false;
	}

	return val > 0;
}
#else
int power_init(void)
{
	LOG_INF("USB VBUS detection not available");
	return -ENOTSUP;
}

bool power_usb_present(void)
{
	return false;
}
#endif

int power_enter_deep_sleep(const struct gpio_dt_spec *wake_button)
{
	esp_err_t err;
	esp_deepsleep_gpio_wake_up_mode_t wake_mode;

	if (wake_button == NULL) {
		return -EINVAL;
	}

	wake_mode = (wake_button->dt_flags & GPIO_ACTIVE_LOW) != 0 ?
		ESP_GPIO_WAKEUP_GPIO_LOW : ESP_GPIO_WAKEUP_GPIO_HIGH;

	err = esp_deep_sleep_enable_gpio_wakeup(1ULL << wake_button->pin, wake_mode);
	if (err != ESP_OK) {
		LOG_ERR("Failed to configure wake GPIO%d: %d",
			wake_button->pin, err);
		return -EINVAL;
	}

	LOG_INF("Entering deep sleep, wake on GPIO%d %s",
		wake_button->pin,
		wake_mode == ESP_GPIO_WAKEUP_GPIO_LOW ? "low" : "high");
	sys_poweroff();

	return 0;
}
