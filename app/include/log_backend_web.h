#ifndef LOG_BACKEND_WEB_H
#define LOG_BACKEND_WEB_H

#include <stddef.h>
#include <stdint.h>

/**
 * Copy a snapshot of the most recent log bytes into @p dst.
 *
 * The ring buffer is NOT consumed. Repeated calls return overlapping data
 * (the rolling window), so the HTTP handler can be polled by a browser.
 */
size_t log_backend_web_snapshot(uint8_t *dst, size_t max);

#endif
