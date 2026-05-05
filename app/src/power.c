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
