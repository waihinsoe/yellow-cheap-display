#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *_loading_icon;
    lv_obj_t *alert_pop_up_close_btn;
    lv_obj_t *alert_pop_up_close_icon;
    lv_obj_t *alert_pop_up_container;
    lv_obj_t *connect_btn;
    lv_obj_t *connect_label;
    lv_obj_t *connect_pop_up_close_btn;
    lv_obj_t *connect_pop_up_close_icon;
    lv_obj_t *connect_pop_up_container;
    lv_obj_t *connecting_label;
    lv_obj_t *data_ui_container;
    lv_obj_t *fail_label;
    lv_obj_t *fail_ui_container;
    lv_obj_t *humi_container;
    lv_obj_t *humi_logo;
    lv_obj_t *humi_value;
    lv_obj_t *keyboard;
    lv_obj_t *light_container;
    lv_obj_t *light_switch;
    lv_obj_t *loading_ui_container;
    lv_obj_t *location_dropdown;
    lv_obj_t *navbar_container;
    lv_obj_t *password_text_area;
    lv_obj_t *pop_navbar_container;
    lv_obj_t *pop_up_close_btn;
    lv_obj_t *pop_up_close_icon;
    lv_obj_t *pop_up_container;
    lv_obj_t *pressure_container;
    lv_obj_t *pressure_value;
    lv_obj_t *setting_btn;
    lv_obj_t *setting_icon;
    lv_obj_t *success_icon;
    lv_obj_t *success_ui_container;
    lv_obj_t *temp_container;
    lv_obj_t *temp_logo;
    lv_obj_t *temp_value;
    lv_obj_t *wifi_label;
    lv_obj_t *wifi_list;
    lv_obj_t *wifi_status;
    lv_obj_t *wifi_switch;
    lv_obj_t *wifi_switch_container;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/