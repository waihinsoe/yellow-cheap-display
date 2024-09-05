// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: SquareLine_Project

#include "ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_wifiSwitch = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_wifiSwitch, 50);
    lv_obj_set_height(ui_wifiSwitch, 25);
    lv_obj_set_x(ui_wifiSwitch, 126);
    lv_obj_set_y(ui_wifiSwitch, -97);
    lv_obj_set_align(ui_wifiSwitch, LV_ALIGN_CENTER);


    ui_wifiSelect = lv_dropdown_create(ui_Screen1);
    lv_dropdown_set_options(ui_wifiSelect, "Option 1\nOption 2\nOption 3");
    lv_obj_set_width(ui_wifiSelect, 223);
    lv_obj_set_height(ui_wifiSelect, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_wifiSelect, -38);
    lv_obj_set_y(ui_wifiSelect, -95);
    lv_obj_set_align(ui_wifiSelect, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_wifiSelect, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags



    ui_connectingContainer = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_connectingContainer);
    lv_obj_set_width(ui_connectingContainer, 141);
    lv_obj_set_height(ui_connectingContainer, 115);
    lv_obj_set_x(ui_connectingContainer, -4);
    lv_obj_set_y(ui_connectingContainer, 16);
    lv_obj_set_align(ui_connectingContainer, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_connectingContainer, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_remove_flag(ui_connectingContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_connectLoading = lv_spinner_create(ui_connectingContainer);
    //lv_spinner_set_anim_params(ui_connectLoading, 1000, 90);
    lv_obj_set_width(ui_connectLoading, 52);
    lv_obj_set_height(ui_connectLoading, 51);
    lv_obj_set_x(ui_connectLoading, 1);
    lv_obj_set_y(ui_connectLoading, -14);
    lv_obj_set_align(ui_connectLoading, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_connectLoading, LV_OBJ_FLAG_CLICKABLE);      /// Flags

    ui_connectingLabel = lv_label_create(ui_connectingContainer);
    lv_obj_set_width(ui_connectingLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_connectingLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_connectingLabel, 4);
    lv_obj_set_y(ui_connectingLabel, 38);
    lv_obj_set_align(ui_connectingLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_connectingLabel, "connecting......");

    ui_keyboardAndTextContainer = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_keyboardAndTextContainer);
    lv_obj_set_width(ui_keyboardAndTextContainer, 320);
    lv_obj_set_height(ui_keyboardAndTextContainer, 160);
    lv_obj_set_x(ui_keyboardAndTextContainer, 1);
    lv_obj_set_y(ui_keyboardAndTextContainer, 39);
    lv_obj_set_align(ui_keyboardAndTextContainer, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_keyboardAndTextContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Keyboard = lv_keyboard_create(ui_keyboardAndTextContainer);
    lv_obj_set_height(ui_Keyboard, 120);
    lv_obj_set_width(ui_Keyboard, lv_pct(100));
    lv_obj_set_x(ui_Keyboard, -2);
    lv_obj_set_y(ui_Keyboard, 18);
    lv_obj_set_align(ui_Keyboard, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Keyboard, LV_OBJ_FLAG_HIDDEN);     /// Flags

    ui_passwordTextAera = lv_textarea_create(ui_keyboardAndTextContainer);
    lv_obj_set_height(ui_passwordTextAera, 36);
    lv_obj_set_width(ui_passwordTextAera, lv_pct(100));
    lv_obj_set_x(ui_passwordTextAera, -1);
    lv_obj_set_y(ui_passwordTextAera, -63);
    lv_obj_set_align(ui_passwordTextAera, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_passwordTextAera, "Enter Password....");



    lv_obj_add_event_cb(ui_wifiSwitch, ui_event_wifiSwitch, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_wifiSelect, ui_event_wifiSelect, LV_EVENT_ALL, NULL);
    lv_keyboard_set_textarea(ui_Keyboard, ui_passwordTextAera);
    lv_obj_add_event_cb(ui_Keyboard, ui_event_Keyboard, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_passwordTextAera, ui_event_passwordTextAera, LV_EVENT_ALL, NULL);

}