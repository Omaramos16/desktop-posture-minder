#include <WiFi.h>
#include <TFT_eSPI.h>
#include <time.h>
#include "secrets.h"   // Include your credentials

// NTP server settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;  // -5 hours in seconds
const int   daylightOffset_sec = 0;  // or adjust for daylight savings time

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(10);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    delay(1000);
    return;
  }
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
  tft.fillScreen(TFT_BLACK);
  tft.drawString(timeStr, 10, 10);
  delay(1000);
}
