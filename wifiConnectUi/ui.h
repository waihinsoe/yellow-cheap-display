// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_comp.h"
#include "ui_comp_hook.h"
#include "ui_events.h"

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
void ui_event_wifiSwitch(lv_event_t * e);
extern lv_obj_t * ui_wifiSwitch;
void ui_event_wifiSelect(lv_event_t * e);
extern lv_obj_t * ui_wifiSelect;
extern lv_obj_t * ui_connectingContainer;
extern lv_obj_t * ui_connectLoading;
extern lv_obj_t * ui_connectingLabel;
extern lv_obj_t * ui_keyboardAndTextContainer;
void ui_event_Keyboard(lv_event_t * e);
extern lv_obj_t * ui_Keyboard;
void ui_event_passwordTextAera(lv_event_t * e);
extern lv_obj_t * ui_passwordTextAera;
extern lv_obj_t * ui____initial_actions0;








void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif