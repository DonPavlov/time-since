#ifndef START_TIME_H
#define START_TIME_H

#include <zephyr/drivers/rtc.h>

/**
 * @brief Start date/time configuration
 *
 * Easily change the start date here. Format is:
 * - Year: actual year (e.g., 2025)
 * - Month: 1-12 (where 1 = January)
 * - Day: 1-31
 * - Hour: 0-23
 * - Minute: 0-59
 * - Second: 0-59
 */

static const struct rtc_time START_TIME = {
	.tm_year = 125,    /* 2025 (2025 - 1900) */
	.tm_mon = 4,       /* May (1-12 converted to 0-11 for struct) */
	.tm_mday = 15,     /* Day 15 */
	.tm_hour = 21,     /* 21:30 German time (CEST) */
	.tm_min = 30,
	.tm_sec = 0,
	.tm_wday = 3,      /* Wednesday (0=Sunday, 3=Wednesday) */
	.tm_yday = 0,      /* Day of year (auto-calculated) */
	.tm_isdst = 1      /* Daylight saving time (1 for summer in Germany, 0 for winter) */
};

#endif /* START_TIME_H */
