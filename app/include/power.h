#ifndef POWER_H
#define POWER_H

#include <stdbool.h>

#include <zephyr/drivers/gpio.h>

int power_init(void);
bool power_usb_present(void);
int power_enter_deep_sleep(const struct gpio_dt_spec *wake_button);

#endif /* POWER_H */
