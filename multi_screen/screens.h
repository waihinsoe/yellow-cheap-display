#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *screen2;
    lv_obj_t *screen3;
    lv_obj_t *label_main_btn_in_screen2;
    lv_obj_t *label_main_btn_in_screen3;
    lv_obj_t *label_main_screen;
    lv_obj_t *label_screen2;
    lv_obj_t *label_screen2_btn_in_main;
    lv_obj_t *label_screen2_btn_in_screen3;
    lv_obj_t *label_screen3;
    lv_obj_t *label_screen3_btn_in_main;
    lv_obj_t *label_screen3_btn_in_screen2;
    lv_obj_t *main_btn_in_screen2;
    lv_obj_t *main_btn_in_screen3;
    lv_obj_t *screen2_btn_in_main;
    lv_obj_t *screen2_btn_in_screen3;
    lv_obj_t *screen3_btn_in_main;
    lv_obj_t *screen3_btn_in_screen2;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SCREEN2 = 2,
    SCREEN_ID_SCREEN3 = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_screen2();
void tick_screen_screen2();

void create_screen_screen3();
void tick_screen_screen3();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/