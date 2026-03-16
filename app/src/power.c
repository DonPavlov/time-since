/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "power.h"

#include <errno.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

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
