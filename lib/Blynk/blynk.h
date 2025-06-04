#ifndef BLYNK_H
#define BLYNK_H

#include <WiFi.h>
#include <BlynkSimpleEsp32_SSL.h>

// Thông tin WiFi và Blynk
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_PASSWORD"

// Virtual Pins
#define VPIN_AIR_QUALITY V0
#define VPIN_FAN_STATE   V1
#define VPIN_THRESHOLD   V2

extern int fanState;
extern int threshold;

void setupBlynk();
void sendAirQuality(int airQualityPPM);

#endif
