/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "time_utils.h"

#include <stdbool.h>
#include <stdio.h>

#include <zephyr/sys/timeutil.h>

#include "start_time.h"

static int last_sunday_mday(int year, int month)
{
	static const uint8_t month_offsets[] = {
		0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4
	};
	const int last_mday = 31;
	int calc_year = year;
	int weekday;

	if (month < 3) {
		calc_year--;
	}

	weekday = (calc_year + calc_year / 4 - calc_year / 100 +
		   calc_year / 400 + month_offsets[month - 1] + last_mday) % 7;

	return last_mday - weekday;
}

static bool berlin_dst_active_for_local_time(const struct rtc_time *timeptr)
{
	const int year = timeptr->tm_year + 1900;
	const int month = timeptr->tm_mon + 1;
	const int mday = timeptr->tm_mday;
	const int hour = timeptr->tm_hour;
	int transition_mday;

	if (month < 3 || month > 10) {
		return false;
	}

	if (month > 3 && month < 10) {
		return true;
	}

	transition_mday = last_sunday_mday(year, month);

	if (month == 3) {
		return mday > transition_mday ||
		       (mday == transition_mday && hour >= 2);
	}

	return mday < transition_mday ||
	       (mday == transition_mday && hour < 3);
}

void time_utils_format_elapsed(uint32_t elapsed, char *buf, size_t len)
{
	uint32_t days = elapsed / 86400U;
	uint32_t hours = (elapsed / 3600U) % 24U;
	uint32_t minutes = (elapsed / 60U) % 60U;
	uint32_t seconds = elapsed % 60U;

	if (buf == NULL || len == 0) {
		return;
	}

	snprintf(buf, len, "%06u\n%02u %02u %02u",
		 days, hours, minutes, seconds);
}

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

time_t time_utils_berlin_local_to_epoch_utc(const struct rtc_time *timeptr)
{
	struct tm local_tm = {
		.tm_sec = timeptr->tm_sec,
		.tm_min = timeptr->tm_min,
		.tm_hour = timeptr->tm_hour,
		.tm_mday = timeptr->tm_mday,
		.tm_mon = timeptr->tm_mon,
		.tm_year = timeptr->tm_year,
		.tm_isdst = 0,
	};
	const time_t local_epoch = timeutil_timegm(&local_tm);
	const time_t utc_offset_seconds =
		berlin_dst_active_for_local_time(timeptr) ? 7200 : 3600;

	return local_epoch - utc_offset_seconds;
}

time_t time_utils_start_epoch_utc(void)
{
	return time_utils_berlin_local_to_epoch_utc(&START_TIME);
}
