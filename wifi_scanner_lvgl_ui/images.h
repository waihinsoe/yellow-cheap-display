#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_wifi_disconnect_icon;
extern const lv_img_dsc_t img_setting_icon;
extern const lv_img_dsc_t img_cross_icon;
extern const lv_img_dsc_t img_wifi_connect_icon;
extern const lv_img_dsc_t img_success_icon;
extern const lv_img_dsc_t img_temp_icon;
extern const lv_img_dsc_t img_humi_icon;
extern const lv_img_dsc_t img_pressure_icon;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[8];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/