#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include <zephyr/drivers/rtc.h>

void time_utils_format_elapsed(uint32_t elapsed, char *buf, size_t len);
time_t time_utils_berlin_local_to_epoch_utc(const struct rtc_time *timeptr);
time_t time_utils_rtc_to_epoch_utc(const struct rtc_time *timeptr);
time_t time_utils_start_epoch_utc(void);

#endif /* TIME_UTILS_H */
