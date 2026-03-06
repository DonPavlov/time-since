/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdio.h>

#include <lvgl.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static const int32_t sleep_time_ms = 50;
static const uint32_t circle_radius = 8U;
static const uint32_t ticks_per_second = 1000U / sleep_time_ms;

static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));

static void update_rtc_label(lv_obj_t *rtc_label)
{
	struct rtc_time tm = { 0 };
	char rtc_buf[16];
	int ret;

	if (!device_is_ready(rtc)) {
		lv_label_set_text(rtc_label, "RTC not ready");
		return;
	}

	ret = rtc_get_time(rtc, &tm);
	if (ret < 0) {
		lv_label_set_text(rtc_label, "RTC read error");
		return;
	}

	snprintf(rtc_buf, sizeof(rtc_buf), "%02d:%02d:%02d",
		 tm.tm_hour, tm.tm_min, tm.tm_sec);
	lv_label_set_text(rtc_label, rtc_buf);
}

int main(void)
{
	uint32_t count = 0;
	uint32_t circle_pos = 0;
	char counter_buf[12] = { 0 };
	const struct device *display;
	struct display_capabilities caps;
	lv_obj_t *hello_label;
	lv_obj_t *counter_label;
	lv_obj_t *rtc_label;
	lv_obj_t *rect;
	lv_obj_t *circle;
	lv_style_t counter_label_style;
	lv_style_t rect_style;
	lv_style_t circle_style;
	static lv_point_precise_t rect_points[] = {
		{ 0, 0 }, { 120, 0 }, { 120, 20 }, { 0, 20 }, { 0, 0 }
	};
	int ret;

	LOG_INF("main() started");

	display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display)) {
		LOG_ERR("display not ready");
		return 0;
	}

	display_get_capabilities(display, &caps);

	hello_label = lv_label_create(lv_screen_active());
	lv_label_set_text(hello_label, "Hello, World!");
	lv_obj_align(hello_label, LV_ALIGN_TOP_MID, 0, 3);

	lv_style_init(&counter_label_style);
	lv_style_set_text_font(&counter_label_style, &lv_font_montserrat_20);

	counter_label = lv_label_create(lv_screen_active());
	lv_obj_add_style(counter_label, &counter_label_style, 0);
	lv_obj_align(counter_label, LV_ALIGN_BOTTOM_RIGHT, -2, -1);
	lv_label_set_text(counter_label, "0");

	rtc_label = lv_label_create(lv_screen_active());
	lv_obj_align(rtc_label, LV_ALIGN_BOTTOM_LEFT, 2, -1);
	lv_label_set_text(rtc_label, "RTC...");
	update_rtc_label(rtc_label);

	lv_style_init(&rect_style);
	lv_style_set_line_width(&rect_style, 2);
	lv_style_set_line_color(&rect_style, lv_color_white());

	rect = lv_line_create(lv_screen_active());
	lv_obj_add_style(rect, &rect_style, 0);
	lv_line_set_points(rect, rect_points, ARRAY_SIZE(rect_points));
	lv_obj_align(rect, LV_ALIGN_TOP_MID, 0, 0);

	lv_style_init(&circle_style);
	lv_style_set_radius(&circle_style, LV_RADIUS_CIRCLE);
	lv_style_set_bg_opa(&circle_style, LV_OPA_COVER);
	lv_style_set_bg_color(&circle_style, lv_color_white());
	lv_style_set_border_width(&circle_style, 0);

	circle = lv_obj_create(lv_screen_active());
	lv_obj_set_size(circle, circle_radius * 2U, circle_radius * 2U);
	lv_obj_add_style(circle, &circle_style, 0);
	lv_obj_align(circle, LV_ALIGN_LEFT_MID, -(int32_t)circle_radius, 0);

	lv_timer_handler();
	ret = display_blanking_off(display);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("display blanking off failed (%d)", ret);
		return 0;
	}

	while (1) {
		if ((count % ticks_per_second) == 0U) {
			snprintf(counter_buf, sizeof(counter_buf), "%u", count / ticks_per_second);
			LOG_INF("tick: %s", counter_buf);
			lv_label_set_text(counter_label, counter_buf);
			update_rtc_label(rtc_label);
		}
		count++;

		if (circle_pos >= caps.x_resolution) {
			circle_pos = 0U;
		}

		lv_obj_align(circle, LV_ALIGN_LEFT_MID,
			     (int32_t)circle_pos - (int32_t)circle_radius, 0);
		circle_pos += MAX(1U, caps.x_resolution / ticks_per_second);

		lv_timer_handler();
		k_msleep(sleep_time_ms);
	}
}
