// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.6
// Project name: M5stackClock

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TimeLabel = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_TimeLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_TimeLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_TimeLabel, 0);
    lv_obj_set_y(ui_TimeLabel, -9);
    lv_obj_set_align(ui_TimeLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_TimeLabel, "12:24");
    lv_obj_set_style_text_color(ui_TimeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_TimeLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui_TimeLabel, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_TimeLabel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_TimeLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_TimeLabel, &ui_font_timeOutlineFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DateLabel = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_DateLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_DateLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_DateLabel, 0);
    lv_obj_set_y(ui_DateLabel, 195);
    lv_obj_set_align(ui_DateLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_DateLabel, "Sun, Mar 2");
    lv_obj_set_style_text_align(ui_DateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_DateLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SecLabel = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_SecLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SecLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SecLabel, 145);
    lv_obj_set_y(ui_SecLabel, 112);
    lv_obj_set_align(ui_SecLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_SecLabel, "30");
    lv_obj_set_style_text_align(ui_SecLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SecLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_amPMLabel = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_amPMLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_amPMLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_amPMLabel, 144);
    lv_obj_set_y(ui_amPMLabel, 129);
    lv_obj_set_align(ui_amPMLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_amPMLabel, "pm");
    lv_obj_set_style_text_align(ui_amPMLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_amPMLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

}
