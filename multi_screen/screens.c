#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_main_page(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    LV_LOG_USER("Clicked");
    loadScreen(SCREEN_ID_MAIN);
  }
}

static void event_handler_page2(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    LV_LOG_USER("Clicked");
    loadScreen(SCREEN_ID_SCREEN2);
  }
}

static void event_handler_page3(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    LV_LOG_USER("Clicked");
    loadScreen(SCREEN_ID_SCREEN3);
  }
}


void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff4a4a), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // label_main_screen
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_main_screen = obj;
            lv_obj_set_pos(obj, 110, 52);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "MAIN SCREEN");
        }
        {
            // screen2_btn_in_main
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.screen2_btn_in_main = obj;
            lv_obj_set_pos(obj, 27, 132);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_screen2_btn_in_main
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_screen2_btn_in_main = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "screen2");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_add_event_cb(obj,event_handler_page2,LV_EVENT_ALL,NULL );
                }
            }
        }
        {
            // screen3_btn_in_main
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.screen3_btn_in_main = obj;
            lv_obj_set_pos(obj, 192, 132);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_screen3_btn_in_main
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_screen3_btn_in_main = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "screen3");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_add_event_cb(obj,event_handler_page3,LV_EVENT_ALL,NULL );
                }
            }
        }
    }
}

void tick_screen_main() {
}

void create_screen_screen2() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.screen2 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfffe4dd3), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // label_screen2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_screen2 = obj;
            lv_obj_set_pos(obj, 120, 51);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Screen 2");
        }
        {
            // main_btn_in_screen2
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.main_btn_in_screen2 = obj;
            lv_obj_set_pos(obj, 28, 134);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_main_btn_in_screen2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_main_btn_in_screen2 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Main screen");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_add_event_cb(obj,event_handler_main_page,LV_EVENT_ALL,NULL );
                }
            }
        }
        {
            // screen3_btn_in_screen2
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.screen3_btn_in_screen2 = obj;
            lv_obj_set_pos(obj, 183, 134);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_screen3_btn_in_screen2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_screen3_btn_in_screen2 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Screen 3");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_add_event_cb(obj,event_handler_page3,LV_EVENT_ALL,NULL );
                }
            }
        }
    }
}

void tick_screen_screen2() {
}

void create_screen_screen3() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.screen3 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffe512), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // label_screen3
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.label_screen3 = obj;
            lv_obj_set_pos(obj, 128, 43);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Screen 3");
        }
        {
            // main_btn_in_screen3
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.main_btn_in_screen3 = obj;
            lv_obj_set_pos(obj, 25, 143);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_main_btn_in_screen3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_main_btn_in_screen3 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Main Screen");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_add_event_cb(obj,event_handler_main_page,LV_EVENT_ALL,NULL );
                }
            }
        }
        {
            // screen2_btn_in_screen3
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.screen2_btn_in_screen3 = obj;
            lv_obj_set_pos(obj, 191, 143);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_screen2_btn_in_screen3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_screen2_btn_in_screen3 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Screen 2");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_add_event_cb(obj,event_handler_page2,LV_EVENT_ALL,NULL );
                }
            }
        }
    }
}

void tick_screen_screen3() {
}


void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_screen2();
    create_screen_screen3();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_screen2,
    tick_screen_screen3,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
