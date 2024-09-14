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
#include "images.h"
#include "WiFi.h"
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

char ssid[100];
char password[100];

// Global variable to track connection attempts
int connection_attempts = 0;

void setup() {

  //Some basic info on the Serial console
  String LVGL_Arduino = "LVGL demo ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);

    // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

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

  lv_obj_add_event_cb(objects.setting_btn, setting_icon_event_handler, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(objects.ssid_text_area, ssid_text_area_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(objects.password_text_area, password_text_area_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(objects.connect_btn, connect_btn_event_handler, LV_EVENT_ALL, NULL);
  
}

void loop() {
  lv_tick_inc(millis() - lastTick);  //Update the tick timer. Tick is new for LVGL 9
  lastTick = millis();
  lv_timer_handler();  //Update the UI
  delay(5);
}


static void setting_icon_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  Serial.println("hello");

  if(code == LV_EVENT_CLICKED) {
    if(lv_obj_has_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN)) {
      lv_obj_clear_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN); // Show the object
    } else {
        lv_obj_add_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN);   // Hide the object
    }
  }
}

static void ssid_text_area_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_FOCUSED) {
    lv_keyboard_set_textarea(objects.keyboard, objects.ssid_text_area);
    lv_obj_clear_flag(objects.keyboard, LV_OBJ_FLAG_HIDDEN);   // Hide the object
  }
  if(code == LV_EVENT_DEFOCUSED || code == LV_EVENT_READY) {
    lv_keyboard_set_textarea(objects.keyboard, NULL);
    lv_obj_add_flag(objects.keyboard, LV_OBJ_FLAG_HIDDEN);   // Hide the object

    const char *txt = lv_textarea_get_text(objects.ssid_text_area);
    strncpy(ssid,txt,sizeof(ssid));
    ssid[sizeof(ssid)-1] ='\0';



    lv_obj_clear_state(objects.ssid_text_area, LV_STATE_FOCUSED); // Manually clear the focus state

  }
}



static void password_text_area_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_FOCUSED) {
    lv_keyboard_set_textarea(objects.keyboard, objects.password_text_area);
    lv_obj_clear_flag(objects.keyboard, LV_OBJ_FLAG_HIDDEN);   // Hide the object
  }
  if(code == LV_EVENT_DEFOCUSED ||  code == LV_EVENT_READY) {
    lv_keyboard_set_textarea(objects.keyboard, NULL);
    lv_obj_add_flag(objects.keyboard, LV_OBJ_FLAG_HIDDEN);   // Hide the object

    const char *txt = lv_textarea_get_text(objects.password_text_area);
    strncpy(password,txt,sizeof(password));
    password[sizeof(password)-1] ='\0';

    

    lv_obj_clear_state(objects.password_text_area, LV_STATE_FOCUSED); // Manually clear the focus state
  }
}


static void connect_btn_event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {
        // Hide the pop-up and show the loading UI
        lv_obj_add_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(objects.loading_ui_container, LV_OBJ_FLAG_HIDDEN);

        Serial.print("SSID: "); Serial.println(ssid);
        Serial.print("PASSWORD: "); Serial.println(password);

        // Begin connection attempt
        WiFi.begin(ssid, password);
        Serial.print("Connecting to "); Serial.println(ssid);

        // Reset connection attempts
        connection_attempts = 0;

        // Start a non-blocking check
        start_wifi_connect_checker();
    }
}

void start_wifi_connect_checker() {
    lv_timer_t * timer = lv_timer_create(wifi_status_checker, 500, NULL);
    lv_timer_ready(timer); // Start the timer immediately
}

void wifi_status_checker(lv_timer_t * timer) {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected successfully");
        Serial.print("IP Address: "); Serial.println(WiFi.localIP());

        // Update the UI to show success
        lv_obj_add_flag(objects.loading_ui_container, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(objects.connected_ui_container, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_src(objects.wifi_status, &img_wifi_connected_icon);

        lv_timer_del(timer); // Stop the timer
    } else {
        // Increase the connection attempts
        connection_attempts++;

        // Check if maximum attempts reached
        if (connection_attempts > 20) { // Adjust max attempts as needed
            Serial.println("\nFailed to connect to WiFi. Please check your credentials or network status.");

            // Update UI to show failure
            lv_obj_add_flag(objects.loading_ui_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN); // Assume there's a failed UI container
            lv_img_set_src(objects.wifi_status, &img_wifi_not_connected_icon);
            lv_timer_del(timer); // Stop the timer
        } else {
            // Still trying to connect, update UI if needed, e.g., spinning a spinner
            Serial.print(".");
        }
    }
}
















