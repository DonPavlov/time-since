#ifndef POWER_H
#define POWER_H

#include <zephyr/drivers/gpio.h>

int power_enter_deep_sleep(const struct gpio_dt_spec *wake_button);

#endif /* POWER_H */
