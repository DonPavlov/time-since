/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gui.h"

#include <stdio.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gui, LOG_LEVEL_DBG);

void gui_init(struct gui_ctx *ctx, bool rtc_has_time, uint32_t initial_elapsed)
{
	if (ctx == NULL) {
		return;
	}

	ctx->last_elapsed = UINT32_MAX;
	ctx->last_wifi_active = true;
	ctx->counter_buf[0] = '\0';

	lv_style_init(&ctx->counter_label_style);
	lv_style_set_text_font(&ctx->counter_label_style, &lv_font_spleen);
	lv_style_set_text_align(&ctx->counter_label_style, LV_TEXT_ALIGN_CENTER);
	lv_style_set_text_line_space(&ctx->counter_label_style, 0);

	ctx->counter_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(ctx->counter_label, &ctx->counter_label_style, 0);
	lv_obj_set_width(ctx->counter_label, LV_PCT(100));
	lv_obj_align(ctx->counter_label, LV_ALIGN_CENTER, 0, 0);

	lv_style_init(&ctx->wifi_label_style);
	lv_style_set_text_font(&ctx->wifi_label_style, &lv_font_icons);

	ctx->wifi_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(ctx->wifi_label, &ctx->wifi_label_style, 0);
	lv_obj_align(ctx->wifi_label, LV_ALIGN_BOTTOM_RIGHT, -2, -3);

	(void)rtc_has_time;
	gui_set_counter(ctx, initial_elapsed);
	gui_set_wifi_active(ctx, false);
}

void gui_set_counter(struct gui_ctx *ctx, uint32_t elapsed)
{
	if (ctx == NULL || ctx->counter_label == NULL ||
	    elapsed == ctx->last_elapsed) {
		return;
	}

	uint32_t days = elapsed / 86400U;
	uint32_t hours = (elapsed / 3600U) % 24U;
	uint32_t minutes = (elapsed / 60U) % 60U;
	uint32_t seconds = elapsed % 60U;

	ctx->last_elapsed = elapsed;
	snprintf(ctx->counter_buf, sizeof(ctx->counter_buf),
		 "%06u\n%02u %02u %02u", days, hours, minutes, seconds);
	lv_label_set_text(ctx->counter_label, ctx->counter_buf);
}

void gui_set_wifi_active(struct gui_ctx *ctx, bool active)
{
	if (ctx == NULL || ctx->wifi_label == NULL ||
	    active == ctx->last_wifi_active) {
		return;
	}

	ctx->last_wifi_active = active;
	lv_label_set_text(ctx->wifi_label, active ? LV_SYMBOL_WIFI : "");
}
