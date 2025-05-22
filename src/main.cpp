
#include "ADC.h"
#include "firebase_module.h"
#include "wifi_module.h"
#include <Arduino.h>

#define WIFI_SSID "MinhDuong"
#define WIFI_PASSWORD "duong123"

#define FIREBASE_HOST "https://dang-minh-duong-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "dURebMzmMPubw62tUbzz3YAkmwRGAE58ChTFhtSX"


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
    sendAirQuality((int)adc_read(6));
}
