/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "time_utils.h"

#include <zephyr/sys/timeutil.h>

#include "start_time.h"

static bool is_leap_year(int year)
{
	if ((year % 4) != 0) {
		return false;
	}

	if ((year % 100) != 0) {
		return true;
	}

	return (year % 400) == 0;
}

static int days_in_month(int year, int month)
{
	static const int month_lengths[] = {
		31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31,
	};

	if (month == 1 && is_leap_year(year)) {
		return 29;
	}

	return month_lengths[month];
}

static int last_sunday_of_month(int year, int month)
{
	struct tm month_end = {
		.tm_year = year - 1900,
		.tm_mon = month,
		.tm_mday = days_in_month(year, month),
		.tm_hour = 12,
		.tm_isdst = 0,
	};
	time_t month_end_epoch = timeutil_timegm(&month_end);
	struct tm *month_end_utc = gmtime(&month_end_epoch);

	if (month_end_utc == NULL) {
		return month_end.tm_mday;
	}

	return month_end.tm_mday - month_end_utc->tm_wday;
}

bool time_utils_berlin_is_dst_utc(time_t utc_epoch)
{
	struct tm *utc = gmtime(&utc_epoch);
	int year;
	int march_switch_day;
	int october_switch_day;

	if (utc == NULL) {
		return false;
	}

	year = utc->tm_year + 1900;

	if (utc->tm_mon < 2 || utc->tm_mon > 9) {
		return false;
	}

	if (utc->tm_mon > 2 && utc->tm_mon < 9) {
		return true;
	}

	march_switch_day = last_sunday_of_month(year, 2);
	october_switch_day = last_sunday_of_month(year, 9);

	if (utc->tm_mon == 2) {
		if (utc->tm_mday > march_switch_day) {
			return true;
		}

		if (utc->tm_mday < march_switch_day) {
			return false;
		}

		return utc->tm_hour >= 1;
	}

	if (utc->tm_mday < october_switch_day) {
		return true;
	}

	if (utc->tm_mday > october_switch_day) {
		return false;
	}

	return utc->tm_hour < 1;
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
