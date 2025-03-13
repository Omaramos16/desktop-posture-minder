#include <lvgl.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include "secrets.h"  
#include "clock_face.h"  // Include the clock face functions

// NTP Server Settings
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000; // UTC-5
const int daylightOffset_sec = 3600;  // Daylight savings

// Use Arduino millis() as the tick source
static uint32_t my_tick(void) {
    return millis();
}

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    // Sync time using NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Initialize LVGL
    lv_init();
    lv_tick_set_cb(my_tick);

    // Initialize the clock face (calls clock_face.cpp)
    create_clock_face();

    Serial.println("Setup complete!");
}

void loop() {
    lv_timer_handler();  // Let LVGL process updates
    delay(5);
}
