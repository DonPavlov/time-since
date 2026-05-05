#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <time.h>

#include <zephyr/drivers/rtc.h>

time_t time_utils_rtc_to_epoch_utc(const struct rtc_time *timeptr);
time_t time_utils_start_epoch_utc(void);

#endif /* TIME_UTILS_H */
