#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stdbool.h>
#include <time.h>

#include <zephyr/drivers/rtc.h>

time_t time_utils_rtc_to_epoch_utc(const struct rtc_time *timeptr);
time_t time_utils_start_epoch_utc(void);
bool time_utils_berlin_is_dst_utc(time_t utc_epoch);

#endif /* TIME_UTILS_H */
