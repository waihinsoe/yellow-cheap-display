#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *connect_btn;
    lv_obj_t *connect_label;
    lv_obj_t *connected_label;
    lv_obj_t *connected_ui_container;
    lv_obj_t *keyboard;
    lv_obj_t *loading_label;
    lv_obj_t *loading_ui_container;
    lv_obj_t *password_text_area;
    lv_obj_t *pop_up_container;
    lv_obj_t *setting_btn;
    lv_obj_t *setting_icon;
    lv_obj_t *ssid_text_area;
    lv_obj_t *wifi_status;
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