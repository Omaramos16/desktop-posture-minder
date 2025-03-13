#include "clock_face.h"
#include <sys/time.h>  // Required for time functions on some ESP32 builds
#include <time.h>      // Provides struct tm and strftime()
#include <WiFi.h>      // Ensure WiFi is included for NTP


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

// Function to update the clock time
void update_time(lv_timer_t *timer) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        lv_label_set_text(clock_label, "Time Error!");
        return;
    }
    
    char timeStr[10];
    strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    lv_label_set_text(clock_label, timeStr);
}

// Function to create the clock face UI
void create_clock_face() {
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
}
