#include "wifi_module.h"
#include <WiFi.h>
#include <Arduino.h>
bool connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 5) {
        delay(1000);
        Serial.print(".");
    }
    if (i==5) {
        Serial.println("\nCan't connect to Wifi");
        return false;
    }
    Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
    return true;
}

