/*
 * In-RAM log backend serving a rolling window of recent log text.
 *
 * Design: a byte-oriented circular buffer. Log messages are formatted by the
 * standard log_output machinery (timestamps + level + formatted text) and
 * appended. When the buffer wraps, oldest bytes are overwritten. The HTTP
 * handler calls log_backend_web_snapshot() to copy out the current window in
 * chronological order without draining, so multiple clients (or browser
 * auto-refresh) can re-read the same tail.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "log_backend_web.h"

#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_backend.h>
#include <zephyr/logging/log_backend_std.h>
#include <zephyr/logging/log_core.h>
#include <zephyr/logging/log_output.h>

#define WEB_LOG_BUF_SIZE 4096

static uint8_t ring[WEB_LOG_BUF_SIZE];
static size_t ring_head;
static bool ring_wrapped;

static K_MUTEX_DEFINE(ring_lock);

static uint8_t line_buf[128];

static uint32_t log_format_current = LOG_OUTPUT_TEXT;

static int web_char_out(uint8_t *data, size_t length, void *ctx)
{
	ARG_UNUSED(ctx);

	k_mutex_lock(&ring_lock, K_FOREVER);
	for (size_t i = 0; i < length; i++) {
		ring[ring_head++] = data[i];
		if (ring_head >= WEB_LOG_BUF_SIZE) {
			ring_head = 0;
			ring_wrapped = true;
		}
	}
	k_mutex_unlock(&ring_lock);

	return (int)length;
}

LOG_OUTPUT_DEFINE(web_log_output, web_char_out, line_buf, sizeof(line_buf));

static void web_log_process(const struct log_backend *const backend,
			    union log_msg_generic *msg)
{
	ARG_UNUSED(backend);

	uint32_t flags = log_backend_std_get_flags();
	log_format_func_t fn = log_format_func_t_get(log_format_current);

	fn(&web_log_output, &msg->log, flags);
}

static void web_log_init(const struct log_backend *const backend)
{
	ARG_UNUSED(backend);
	log_output_ctx_set(&web_log_output, NULL);
}

static void web_log_panic(const struct log_backend *const backend)
{
	ARG_UNUSED(backend);
	log_backend_std_panic(&web_log_output);
}

static void web_log_dropped(const struct log_backend *const backend, uint32_t cnt)
{
	ARG_UNUSED(backend);
	log_backend_std_dropped(&web_log_output, cnt);
}

static int web_log_format_set(const struct log_backend *const backend, uint32_t log_type)
{
	ARG_UNUSED(backend);
	log_format_current = log_type;
	return 0;
}

static const struct log_backend_api web_log_api = {
	.process = web_log_process,
	.panic = web_log_panic,
	.init = web_log_init,
	.dropped = web_log_dropped,
	.format_set = web_log_format_set,
};

LOG_BACKEND_DEFINE(log_backend_web, web_log_api, true);

size_t log_backend_web_snapshot(uint8_t *dst, size_t max)
{
	if (dst == NULL || max == 0) {
		return 0;
	}

	k_mutex_lock(&ring_lock, K_FOREVER);

	size_t available = ring_wrapped ? WEB_LOG_BUF_SIZE : ring_head;
	size_t copy = MIN(available, max);

	if (!ring_wrapped) {
		memcpy(dst, ring, copy);
	} else {
		/* chronological order = [head..end) then [0..head) */
		size_t tail_len = WEB_LOG_BUF_SIZE - ring_head;
		if (copy <= tail_len) {
			/* snapshot smaller than tail slice: take the newest `copy`
			 * bytes, which sit at (ring_head - copy) wrapped.
			 */
			size_t start = (ring_head + WEB_LOG_BUF_SIZE - copy) % WEB_LOG_BUF_SIZE;
			if (start + copy <= WEB_LOG_BUF_SIZE) {
				memcpy(dst, &ring[start], copy);
			} else {
				size_t first = WEB_LOG_BUF_SIZE - start;
				memcpy(dst, &ring[start], first);
				memcpy(dst + first, ring, copy - first);
			}
		} else {
			/* full window fits: [head..end) then [0..head) */
			memcpy(dst, &ring[ring_head], tail_len);
			memcpy(dst + tail_len, ring, copy - tail_len);
		}
	}

	k_mutex_unlock(&ring_lock);
	return copy;
}
