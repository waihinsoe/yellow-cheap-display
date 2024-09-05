/* 
 * This assumes you already have LVGL working with Arduino/ESP32
	- Arduino 	
		- C:\Users\kolap\Documents\Arduino
	- EEZ Studio 
		- C:\Users\kolap\eez-projects
		- C:\Users\kolap\Documents\Arduino\EEZ_studio_projects
	- Square Line
		- C:\Users\kolap\Documents\SquareLine\tutorials
		- C:\Users\kolap\Documents\SquareLine\tutorials\Export_UI_files		
		- C:\Users\kolap\Documents\Arduino\SQ_line_CYD
 1. Create UI with Squareline
  - Remember to set both screen width/height and Depth=16 bits as used by ESP32
  - File > Project settings set the following
    - Project Export Root
    - UI Files Export Path (This will contain files you will need to copy over to Arduino project)
      - Remember this location as you will need to copy the contents.
  - Export the UI code
    - Export > Export UI Files
      - This will create files in UI Files Export Path you created above
  - Create a new Arduino project 
	- Open an existing one
  - Copy all UI Files to the Arduino project root
  - In Arduino setup() add:
    - ui_init(); 
    - Run the program
 */

#include <lvgl.h>
#include "ui.h"

#include <TFT_eSPI.h>

#include <XPT2046_Touchscreen.h>
// A library for interfacing with the touch screen
//
// Can be installed from the library manager (Search for "XPT2046")
//https://github.com/PaulStoffregen/XPT2046_Touchscreen
// ----------------------------
// Touch Screen pins
// ----------------------------

// The CYD touch uses some non default
// SPI pins

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240, touchScreenMaximumY = 3800;

/*Set to your screen resolution*/
#define TFT_HOR_RES 320
#define TFT_VER_RES 240

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  if (touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    //Some very basic auto calibration so it doesn't go out of range
    if (p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
    if (p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
    if (p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
    if (p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;
    //Map this to the pixel position
    data->point.x = map(p.x, touchScreenMinimumX, touchScreenMaximumX, 1, TFT_HOR_RES); /* Touchscreen X calibration */
    data->point.y = map(p.y, touchScreenMinimumY, touchScreenMaximumY, 1, TFT_VER_RES); /* Touchscreen Y calibration */
    data->state = LV_INDEV_STATE_PRESSED;
    /*
    Serial.print("Touch x ");
    Serial.print(data->point.x);
    Serial.print(" y ");
    Serial.println(data->point.y);
    */
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

lv_indev_t *indev;      //Touchscreen input device
uint8_t *draw_buf;      //draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0;  //Used to track the tick timer


void setup() {

  //Some basic info on the Serial console
  String LVGL_Arduino = "LVGL demo ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);

  //Initialise the touchscreen
  touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  touchscreen.begin(touchscreenSpi);                                         /* Touchscreen init */
  touchscreen.setRotation(3);                                                /* Inverted landscape orientation to match screen */

  //Initialise LVGL GUI
  lv_init();

  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  lv_display_t *disp;
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);

  //Initialize the XPT2046 input device driver
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  //Done
  Serial.println("LVGL Setup done");

  //Integrate EEZ Studio GUI
  ui_init();

  lv_obj_add_event_cb(objects.btn_clicked, button_click_event_handler, LV_EVENT_CLICKED,NULL);
  lv_obj_add_event_cb(objects.btn_long_click, button_long_click_event_handler, LV_EVENT_LONG_PRESSED, objects.label_click_result);
  lv_obj_add_event_cb(objects.btn_all_event, button_all_event_handler, LV_EVENT_ALL, objects.label_click_result);
  lv_obj_add_event_cb(objects.btn_change_colour, button_change_colour_event_handler, LV_EVENT_CLICKED, objects.btn_change_colour);

  
}

void loop() {
  lv_tick_inc(millis() - lastTick);  //Update the tick timer. Tick is new for LVGL 9
  lastTick = millis();
  lv_timer_handler();  //Update the UI
  delay(5);
}

static void button_click_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *label = (lv_obj_t *) lv_event_get_user_data(e);

  if(label != NULL) {
    lv_label_set_text(label, "Event: LV_EVENT_CLICKED");
  } else{
    LV_LOG_USER("Label not received");
    lv_label_set_text(objects.label_click_result, "Label Not Received");
  }
}

static void button_long_click_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);

  lv_label_set_text(label, "Event: LV_EVENT_LONG_PRESSED");
}

static void button_all_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);

  LV_LOG_USER("CODE: "+ code);

  switch (code) {
    case LV_EVENT_CLICKED:
      lv_label_set_text(label, "Event: LV_EVENT_CLICKED");
      break;
    case LV_EVENT_PRESSED:
      lv_label_set_text(label, "Event : LV_EVENT_PRESSED");
      break;
    case LV_EVENT_RELEASED:
      lv_label_set_text(label, "Event: LV_EVENT_RELEASED");
      break;
    case LV_EVENT_LONG_PRESSED:
      lv_label_set_text(label, "Event: LV_EVENT_LONG_PRESSED");
      break;
    case LV_EVENT_LONG_PRESSED_REPEAT:
      lv_label_set_text(label, "Event: LV_EVENT_LONG_PRESSED_REPEAT");
      break;
    default:
      break;
  }
}

static void button_change_colour_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_user_data(e);

  lv_color_t randomColor = generateRandomColor();
  lv_obj_set_style_bg_color(objects.main,randomColor,LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(objects.label_click_result, "Event: LV_EVENT_CLICKED COLOUR BTN");
}

static lv_color_t generateRandomColor() {
  int r = random(0, 256);
  int g = random(0, 256);
  int b = random(0, 256);

  return lv_color_make(r,g,b);
}















