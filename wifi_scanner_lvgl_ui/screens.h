#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *navbar_container;
    lv_obj_t *pop_navbar_container;
    lv_obj_t *pop_up_close_btn;
    lv_obj_t *pop_up_close_icon;
    lv_obj_t *pop_up_container;
    lv_obj_t *setting_btn;
    lv_obj_t *setting_icon;
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