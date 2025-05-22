
#include "ADC.h"
#include "firebase_module.h"
#include "wifi_module.h"
#include <Arduino.h>
#include <stdint.h>
#define WIFI_SSID "MinhDuong"
#define WIFI_PASSWORD "duong123"

#define FIREBASE_HOST "https://dang-minh-duong-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "dURebMzmMPubw62tUbzz3YAkmwRGAE58ChTFhtSX"

#define ATTEN_0DB                  0x0       // 00: 0 dB
#define ATTEN_3DB                  0x1       // 01: 3 dB ()
#define ATTEN_6DB                  0x2       // 10: 6 dB
#define ATTEN_11DB                 0x3       // 11: 1 dB ()

bool firebaseIsReady = false;
void setup()
{
    adc_init();
    Serial.begin(9600);
    bool isConnectedWifi = connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
    if (isConnectedWifi) firebaseIsReady = setupFirebase(FIREBASE_HOST, FIREBASE_AUTH);
    if (firebaseIsReady) Serial.println("Connected to Firebase");
    else Serial.println("Can't connect to Firebase");
    
}

void loop()
{
    uint16_t adc_value = adc_read(6, ATTEN_11DB);
    sendAirQuality((int)adc_value);
}
