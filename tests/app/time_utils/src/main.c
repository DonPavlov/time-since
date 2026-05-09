/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <time.h>

#include <zephyr/drivers/rtc.h>
#include <zephyr/sys/timeutil.h>
#include <zephyr/ztest.h>

#include "start_time.h"
#include "time_utils.h"

static time_t utc_epoch(int year, int month, int mday,
			int hour, int minute, int second)
{
	struct tm utc_tm = {
		.tm_year = year - 1900,
		.tm_mon = month - 1,
		.tm_mday = mday,
		.tm_hour = hour,
		.tm_min = minute,
		.tm_sec = second,
		.tm_isdst = 0,
	};

	return timeutil_timegm(&utc_tm);
}

static struct rtc_time rtc_local(int year, int month, int mday,
				 int hour, int minute, int second)
{
	return (struct rtc_time) {
		.tm_year = year - 1900,
		.tm_mon = month - 1,
		.tm_mday = mday,
		.tm_hour = hour,
		.tm_min = minute,
		.tm_sec = second,
	};
}

ZTEST(time_utils, test_berlin_summer_start_uses_utc_plus_2)
{
	struct rtc_time local = rtc_local(2025, 5, 15, 22, 0, 0);

	zassert_equal(time_utils_berlin_local_to_epoch_utc(&local),
		      utc_epoch(2025, 5, 15, 20, 0, 0),
		      "Berlin summer local time should use UTC+2");
}

ZTEST(time_utils, test_berlin_winter_start_uses_utc_plus_1)
{
	struct rtc_time local = rtc_local(2025, 1, 15, 22, 0, 0);

	zassert_equal(time_utils_berlin_local_to_epoch_utc(&local),
		      utc_epoch(2025, 1, 15, 21, 0, 0),
		      "Berlin winter local time should use UTC+1");
}

ZTEST(time_utils, test_configured_start_time_is_berlin_local)
{
	zassert_equal(time_utils_start_epoch_utc(),
		      time_utils_berlin_local_to_epoch_utc(&START_TIME),
		      "configured START_TIME should be interpreted as Berlin local");
}

ZTEST(time_utils, test_elapsed_format_zero)
{
	char buf[32];

	time_utils_format_elapsed(0, buf, sizeof(buf));

	zassert_true(strcmp(buf, "000000\n00 00 00") == 0,
		     "unexpected elapsed string: %s", buf);
}

ZTEST(time_utils, test_elapsed_format_minutes_since_same_local_time)
{
	char buf[32];

	time_utils_format_elapsed(17U * 60U + 42U, buf, sizeof(buf));

	zassert_true(strcmp(buf, "000000\n00 17 42") == 0,
		     "unexpected elapsed string: %s", buf);
}

ZTEST(time_utils, test_elapsed_format_day_plus_remainder)
{
	char buf[32];
	uint32_t elapsed = 86400U + 22U * 3600U + 45U * 60U;

	time_utils_format_elapsed(elapsed, buf, sizeof(buf));

	zassert_true(strcmp(buf, "000001\n22 45 00") == 0,
		     "unexpected elapsed string: %s", buf);
}

ZTEST_SUITE(time_utils, NULL, NULL, NULL, NULL, NULL);
