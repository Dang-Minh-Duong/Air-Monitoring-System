#include "blynk.h"

int fanState = 0;
int threshold = 200;  // Ngưỡng mặc định

BLYNK_WRITE(VPIN_FAN_STATE) {
  fanState = param.asInt();  // Bật/Tắt quạt từ app
}

BLYNK_WRITE(VPIN_THRESHOLD) {
  threshold = param.asInt();  // Cập nhật ngưỡng cảnh báo
}

void setupBlynk() {
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
}

void sendAirQuality(int airQualityPPM) {
  Blynk.virtualWrite(VPIN_AIR_QUALITY, airQualityPPM);
}