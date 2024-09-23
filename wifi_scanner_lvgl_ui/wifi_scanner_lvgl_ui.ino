
#define DEBUG  // comment and uncomment to debung

#include "WiFi.h"
#include <lvgl.h>
#include "ui.h"
#include "images.h"
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//********************* http setup *****************************
const char* serverURL = "http://192.168.1.9:3000";


//********************* mqtt setup ******************************

const char* mqtt_broker = "162.254.34.64"; // The IP address of your Mosquitto broker
const int mqtt_port = 1883;
const char* mqtt_username = "lucky7n1";
const char* mqtt_password = "P@ssw0rd";


WiFiClient espClient;
PubSubClient client(espClient);

//********************* mqtt setup ******************************


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

String ssid;
char password[100];
int connection_attempts = 0;
int location_status = 1;
StaticJsonDocument<512> doc;  // Adjust size based on your payload's typical size

// ******************** function decleration ****************************
void updateTemperatureHumidityUI(const JsonDocument& doc);
void updatePressureUI(const JsonDocument& doc);
void processLightStatus(const JsonDocument& doc);
void reconnect();
void start_wifi_connect_checker();
void wifi_status_checker(lv_timer_t * timer);
void updateWiFiScanResults();

static void setting_icon_event_handler(lv_event_t *e);
static void pop_up_close_event_handler(lv_event_t *e);
static void wifi_switch_event_handler(lv_event_t *e);
static void light_switch_event_handler(lv_event_t *e);
static void location_dropdown_event_handler(lv_event_t *e);
static void list_btn_event_handler(lv_event_t *e);
static void connect_pop_up_close_event_handler(lv_event_t *e);
static void password_text_area_event_handler(lv_event_t *e);
static void connect_btn_event_handler(lv_event_t *e);
static void alert_pop_up_close_btn_event_handler(lv_event_t *e);

void callback(char* topic, byte* payload, unsigned int length);

void setup() {

  //Some basic info on the Serial console
  #ifdef DEBUG
  String LVGL_Arduino = "LVGL demo ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);
  #endif
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

  #ifdef DEBUG
  Serial.println("LVGL Setup done");
  #endif
  //Integrate EEZ Studio GUI
  ui_init();

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  lv_obj_add_event_cb(objects.setting_btn, setting_icon_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(objects.pop_up_close_btn, pop_up_close_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(objects.wifi_switch, wifi_switch_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(objects.connect_pop_up_close_btn, connect_pop_up_close_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(objects.password_text_area, password_text_area_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(objects.connect_btn, connect_btn_event_handler, LV_EVENT_ALL ,NULL);
  lv_obj_add_event_cb(objects.alert_pop_up_close_btn,alert_pop_up_close_btn_event_handler, LV_EVENT_ALL,NULL);
  lv_obj_add_event_cb(objects.light_switch,light_switch_event_handler, LV_EVENT_ALL,NULL);
  lv_obj_add_event_cb(objects.location_dropdown,location_dropdown_event_handler, LV_EVENT_VALUE_CHANGED,NULL);
  

}

void loop() {
  lv_tick_inc(millis() - lastTick);  //Update the tick timer. Tick is new for LVGL 9
  lastTick = millis();
  lv_timer_handler();  //Update the UI
  updateWiFiScanResults();

  if (WiFi.status() == WL_CONNECTED) {  // Check if WiFi is connected
    if (!client.connected()) {
      reconnect();  // Attempt to connect to MQTT if not already connected
    }
    client.loop();  // Process MQTT loop
  } 
  else {
    // #ifdef DEBUG
    // Serial.println("WiFi not connected. Waiting to retry...");
    // #endif
  }

  delay(5);
}

void callback(char* topic, byte* payload, unsigned int length) {
  #ifdef DEBUG
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  #endif

  bool isIndoor = strstr(topic,"indoor/esp8266_A") != NULL;
  bool isOutdoor = strstr(topic,"outdoor/esp8266_B") != NULL;

  if((location_status == 1 && isIndoor) || (location_status == 2 && isOutdoor)) {
    char buf[length+1];
    memcpy(buf, payload, length);
    buf[length] = '\0';

    auto error = deserializeJson(doc,buf);

    if(error) {
      #ifdef DEBUG
      Serial.print("DeserializeJson() failed: ");
      Serial.println(error.f_str());
      #endif
      return;
    }

    if (strcmp(topic, "indoor/esp8266_A/light/status") == 0 || strcmp(topic, "outdoor/esp8266_B/light/status") == 0) {
      processLightStatus(doc);
    } else if (strcmp(topic, "indoor/esp8266_A/dht22") == 0 || strcmp(topic, "outdoor/esp8266_B/dht22") == 0 ) {
      updateTemperatureHumidityUI(doc);
    } else if (strcmp(topic, "indoor/esp8266_A/bmp180") == 0 || strcmp(topic, "outdoor/esp8266_B/bmp180") == 0 ) {
      updatePressureUI(doc);
    }

  }else {
    #ifdef DEBUG
    Serial.println("Received topic not relavant to the current context.");
    #endif
  }
}

void updateTemperatureHumidityUI(const JsonDocument& doc) {
  if (doc.containsKey("temperature") && doc.containsKey("humidity")) {
    // Use snprintf instead of String class
    int temp = doc["temperature"].as<int>();
    char tempLabelText[10];
    snprintf(tempLabelText, sizeof(tempLabelText), "%d", temp);
    lv_label_set_text(objects.temp_value, tempLabelText);

    int humi = doc["humidity"].as<int>();
    char humiLabelText[10];
    snprintf(humiLabelText, sizeof(humiLabelText), "%d", humi);
    lv_label_set_text(objects.humi_value, humiLabelText);
  }
}

void updatePressureUI(const JsonDocument& doc) {
  if (doc.containsKey("pressure")) {
    long pressure = doc["pressure"].as<long>();
    char pressureLabelText[15];
    snprintf(pressureLabelText, sizeof(pressureLabelText), "%ld", pressure);
    lv_label_set_text(objects.pressure_value, pressureLabelText);
  }
}

void processLightStatus(const JsonDocument& doc) {
  if (doc.containsKey("status")) {
    int status = doc["status"].as<int>();
    if (status) {
      lv_obj_add_state(objects.light_switch, LV_STATE_CHECKED);
    } else {
      lv_obj_clear_state(objects.light_switch, LV_STATE_CHECKED);
    }
  }
}


void reconnect() {
  static unsigned long last_try = 0;
  unsigned long now = millis();
  //************* non-blocking **********************

  if(now - last_try > 5000) {
    last_try = now;
    if(!client.connected()) {
      #ifdef DEBUG
      Serial.println("Attempting MQTT connection...");
      #endif
      if (client.connect("ESP32_CYD",mqtt_username, mqtt_password)) {
        #ifdef DEBUG
        Serial.println("connected");
        #endif
        client.publish("ESP32_CYD", "hello from ESP32");
        client.subscribe("indoor/esp8266_A/bmp180");
        client.subscribe("indoor/esp8266_A/dht22");
        client.subscribe("indoor/esp8266_A/light/status");

        client.subscribe("outdoor/esp8266_B/bmp180");
        client.subscribe("outdoor/esp8266_B/dht22");
        client.subscribe("outdoor/esp8266_B/light/status");

        client.publish("last_updated_data/indoor","1");
      } else {
        #ifdef DUBUG
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        #endif
      }
    }

  }

  //************* blocking **********************
  // while (!client.connected()) {
  //   Serial.print("Attempting MQTT connection...");
  //   if (client.connect("ESP32Client",mqtt_username, mqtt_password)) {
  //     Serial.println("connected");
  //     client.publish("ESP32_CYD", "hello from ESP32");
  //     client.subscribe("esp01_A/dht22");
  //     client.subscribe("esp01_B/bmp180");
  //     client.subscribe("esp01_C/light/status");
  //   } else {
  //     Serial.print("failed, rc=");
  //     Serial.print(client.state());
  //     Serial.println(" try again in 5 seconds");
  //   }
  // }

  
}

static void setting_icon_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    lv_obj_clear_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN); // Show the object
  }

}

static void pop_up_close_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    lv_obj_add_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN); // Show the object
  }

}


static void wifi_switch_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    if (lv_obj_has_state(objects.wifi_switch, LV_STATE_CHECKED)) {
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      lv_obj_clean(objects.wifi_list);
      lv_list_add_text(objects.wifi_list, "Scanning for WiFi networks...");
      #ifdef DEBUG
      Serial.println("Scanning for WiFi networks...");
      // This is just to update the status; actual completion is checked elsewhere
      Serial.println("Status: ON");
      #endif
      // Start an asynchronous WiFi network scan
      WiFi.scanNetworks(true);


    } else {
      WiFi.disconnect();
      lv_obj_clean(objects.wifi_list);
      #ifdef DEBUG
      Serial.println("Status: OFF");
      #endif
    }
  }
}

static void light_switch_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    if (lv_obj_has_state(objects.light_switch, LV_STATE_CHECKED)) {
      if(location_status == 1) {
        client.publish("indoor/esp8266_A/light/status","{\"status\": 1}");
      }else if(location_status ==2) {
        client.publish("outdoor/esp8266_B/light/status","{\"status\": 1}");
      }
    } else {
      if(location_status == 1) {
        client.publish("indoor/esp8266_A/light/status","{\"status\": 0}");
      }else if(location_status ==2) {
        client.publish("outdoor/esp8266_B/light/status","{\"status\": 0}");
      }
    }
  }
}


static void location_dropdown_event_handler(lv_event_t *e) {
  char buf[32];
  lv_dropdown_get_selected_str(objects.location_dropdown, buf, sizeof(buf));
  
  if(strcmp(buf, "indoor") == 0) {
    printf("indoor is selected!\n");
    location_status = 1;
    client.publish("last_updated_data/indoor","1");
  }else if(strcmp(buf, "outdoor") == 0) {
    printf("outdoor is selected!\n");   
    location_status = 2;
    client.publish("last_updated_data/outdoor","1");
  }
}


// This function should be called in your main loop or a periodic timer
void updateWiFiScanResults() {
  if (WiFi.scanComplete() >= 0) {
    lv_obj_clean(objects.wifi_list);
    int numberOfNetworks = WiFi.scanComplete();
    if (numberOfNetworks == 0) {
      #ifdef DEBUG
      Serial.println("No networks found");
      #endif
      lv_list_add_text(objects.wifi_list, "No WiFi networks found.");
    } else {
      #ifdef DEBUG
      Serial.print(numberOfNetworks);
      Serial.println(" networks found");
      #endif
      for (int i = 0; i < numberOfNetworks; ++i) {
        #ifdef DEBUG
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm)");
        #endif

        lv_obj_t* btn = lv_list_add_button(objects.wifi_list, LV_SYMBOL_WIFI, WiFi.SSID(i).c_str());
        lv_obj_add_event_cb(btn, list_btn_event_handler, LV_EVENT_CLICKED, btn);
      }
    }
    #ifdef DUBUG
    Serial.println("Scan complete");
    #endif
    // Clear the scan results to ready for the next scan
    WiFi.scanDelete();
  }
}

static void list_btn_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_user_data(e);
  if(code == LV_EVENT_CLICKED) {
    ssid = lv_list_get_button_text(objects.wifi_list,btn);
    #ifdef DEBUG
    Serial.println(ssid);
    #endif
    lv_obj_clear_flag(objects.connect_pop_up_container, LV_OBJ_FLAG_HIDDEN);
  }
}

static void connect_pop_up_close_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    lv_obj_add_flag(objects.connect_pop_up_container, LV_OBJ_FLAG_HIDDEN); // Show the object
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
        // // Hide the pop-up and show the loading UI
        // lv_obj_add_flag(objects.pop_up_container, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(objects.alert_pop_up_container, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(objects.loading_ui_container, LV_OBJ_FLAG_HIDDEN);
        #ifdef DEBUG
        Serial.print("SSID: "); Serial.println(ssid);
        Serial.print("PASSWORD: "); Serial.println(password);
        #endif
        // Begin connection attempt
        WiFi.begin(ssid.c_str(), password);
        #ifdef DEBUG
        Serial.print("Connecting to "); Serial.println(ssid);
        #endif
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
        #ifdef DUBUG
        Serial.println("\nWiFi connected successfully");
        Serial.print("IP Address: "); Serial.println(WiFi.localIP());
        #endif
        // Update the UI to show success
        lv_obj_add_flag(objects.loading_ui_container, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(objects.success_ui_container, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_src(objects.wifi_status, &img_wifi_connect_icon);

        lv_timer_del(timer); // Stop the timer
    } else {
        // Increase the connection attempts
        connection_attempts++;

        // Check if maximum attempts reached
        if (connection_attempts > 20) { // Adjust max attempts as needed
            #ifdef DEBUG
            Serial.println("\nFailed to connect to WiFi. Please check your credentials or network status.");
            #endif
            // Update UI to show failure
            lv_obj_add_flag(objects.loading_ui_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.success_ui_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.fail_ui_container, LV_OBJ_FLAG_HIDDEN); // Assume there's a failed UI container
            lv_img_set_src(objects.wifi_status, &img_wifi_disconnect_icon);
            lv_timer_del(timer); // Stop the timer
        } else { 
            #ifdef DEBUG
            // Still trying to connect, update UI if needed, e.g., spinning a spinner
            Serial.print(".");
            #endif
        }
    }
}


static void alert_pop_up_close_btn_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    lv_obj_add_flag(objects.alert_pop_up_container, LV_OBJ_FLAG_HIDDEN); // Show the object
    lv_obj_add_flag(objects.success_ui_container, LV_OBJ_FLAG_HIDDEN); // Show the object
    lv_obj_add_flag(objects.fail_ui_container, LV_OBJ_FLAG_HIDDEN); // Show the object
  }

}











