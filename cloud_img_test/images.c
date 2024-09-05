#include "images.h"

const ext_img_desc_t images[5] = {
    { "rain", &img_rain },
    { "temp", &img_temp },
    { "humi", &img_humi },
    { "pressure", &img_pressure },
    { "sunny", &img_sunny },
};
