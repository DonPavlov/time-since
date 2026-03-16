#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include <stdint.h>

#include <lvgl.h>

LV_FONT_DECLARE(lv_font_font04b03);
LV_FONT_DECLARE(lv_font_icons);
LV_FONT_DECLARE(lv_font_spleen);


struct gui_ctx {
	lv_obj_t *counter_label;
	lv_obj_t *wifi_label;
	lv_style_t counter_label_style;
	lv_style_t wifi_label_style;
	char counter_buf[32];
	uint32_t last_elapsed;
};

void gui_init(struct gui_ctx *ctx, bool rtc_has_time, uint32_t initial_elapsed);
void gui_show_syncing(struct gui_ctx *ctx);
void gui_set_counter(struct gui_ctx *ctx, uint32_t elapsed);
void gui_set_wifi_active(struct gui_ctx *ctx, bool active);

#endif /* GUI_H */
