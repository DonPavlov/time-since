#ifndef START_TIME_H
#define START_TIME_H

#include <zephyr/drivers/rtc.h>

/**
 * @brief Start date/time configuration (treated as Berlin local time)
 *
 * Fields:
 * - tm_year: year since 1900 (2025 -> 125)
 * - tm_mon:  0-indexed (0=Jan, 4=May, 11=Dec)
 * - tm_mday: 1-31
 * - tm_hour: 0-23
 * - tm_min:  0-59
 * - tm_sec:  0-59
 */

static const struct rtc_time START_TIME = {
	.tm_year = 125,    /* 2025 */
	.tm_mon = 4,       /* May */
	.tm_mday = 15,
	.tm_hour = 22,
	.tm_min = 0,
	.tm_sec = 0,
};

#endif /* START_TIME_H */
