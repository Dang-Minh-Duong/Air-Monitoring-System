#ifndef FIREBASE_MODULE_H
#define FIREBASE_H
#include <Arduino.h>

bool setupFirebase(const char* url, const char* token);
bool sendAirQuality(int ppm);
bool sendLog(String deviceID, String state);

#endif
