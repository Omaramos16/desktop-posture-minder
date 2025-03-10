#include <lvgl.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include "secrets.h"  // Contains `ssid` and `password`

// NTP Server Settings
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000; // UTC-5
const int daylightOffset_sec = 0;  // No daylight savings

// Display Settings
#define TFT_HOR_RES   320
#define TFT_VER_RES   480
#define TFT_ROTATION  LV_DISPLAY_ROTATION_90
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

// LVGL Buffers
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
lv_display_t *disp;
lv_obj_t *clock_label;

// Display flush function for LVGL
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    lv_display_flush_ready(disp);
}

// Function to fetch and update time
void update_time(lv_timer_t *timer) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        lv_label_set_text(clock_label, "Time Error!");
        return;
    }
    
    char timeStr[10];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    lv_label_set_text(clock_label, timeStr);
}

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

    // Initialize TFT_eSPI with LVGL
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

    // Create a label for the digital clock
    clock_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(clock_label, &lv_font_montserrat_14, 0);
    lv_obj_align(clock_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(clock_label, "00:00:00");

    // Timer to update the time every second
    lv_timer_create(update_time, 1000, NULL);

    Serial.println("Setup complete!");
}

void loop() {
    lv_timer_handler();  // Let LVGL process updates
    delay(5);
}
