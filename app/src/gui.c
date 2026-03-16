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
	ctx->counter_buf[0] = '\0';

	lv_style_init(&ctx->counter_label_style);
	lv_style_set_text_font(&ctx->counter_label_style, &lv_font_spleen);

	ctx->counter_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(ctx->counter_label, &ctx->counter_label_style, 0);
	lv_obj_align(ctx->counter_label, LV_ALIGN_CENTER, 0, 0);

	lv_style_init(&ctx->wifi_label_style);
	lv_style_set_text_font(&ctx->wifi_label_style, &lv_font_icons);

	ctx->wifi_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(ctx->wifi_label, &ctx->wifi_label_style, 0);
	lv_obj_align(ctx->wifi_label, LV_ALIGN_BOTTOM_RIGHT, -2, -3);

	(void)rtc_has_time;
	gui_set_counter(ctx, initial_elapsed);
	gui_set_wifi_active(ctx, true);
}

void gui_show_syncing(struct gui_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	lv_label_set_text(ctx->counter_label, "Syncing...");
}

void gui_set_counter(struct gui_ctx *ctx, uint32_t elapsed)
{
	if (ctx == NULL || elapsed == ctx->last_elapsed) {
		return;
	}

	ctx->last_elapsed = elapsed;
	snprintf(ctx->counter_buf, sizeof(ctx->counter_buf), "%u", elapsed);
	lv_label_set_text(ctx->counter_label, ctx->counter_buf);
}

void gui_set_wifi_active(struct gui_ctx *ctx, bool active)
{
	if (ctx == NULL) {
		return;
	}

	lv_label_set_text(ctx->wifi_label, active ? LV_SYMBOL_WIFI : "");
}
