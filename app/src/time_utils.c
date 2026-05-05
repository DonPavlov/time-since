/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "time_utils.h"

#include <zephyr/sys/timeutil.h>

#include "start_time.h"

time_t time_utils_rtc_to_epoch_utc(const struct rtc_time *timeptr)
{
	struct tm utc_tm = {
		.tm_sec = timeptr->tm_sec,
		.tm_min = timeptr->tm_min,
		.tm_hour = timeptr->tm_hour,
		.tm_mday = timeptr->tm_mday,
		.tm_mon = timeptr->tm_mon,
		.tm_year = timeptr->tm_year,
		.tm_isdst = 0,
	};

	return timeutil_timegm(&utc_tm);
}

time_t time_utils_start_epoch_utc(void)
{
	struct tm start_tm = {
		.tm_sec = START_TIME.tm_sec,
		.tm_min = START_TIME.tm_min,
		.tm_hour = START_TIME.tm_hour,
		.tm_mday = START_TIME.tm_mday,
		.tm_mon = START_TIME.tm_mon,
		.tm_year = START_TIME.tm_year,
		.tm_isdst = 0,
	};

	return timeutil_timegm(&start_tm);
}
