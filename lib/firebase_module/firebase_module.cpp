#include "firebase_module.h"
#include <addons/TokenHelper.h>
#include <Firebase_ESP_Client.h>
#include <Arduino.h>
#include <stdint.h>
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

String getISOTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
        return "N/A";
    char buf[30];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    return String(buf);
}


bool setupFirebase(const char *url, const char *token)
{
    config.database_url = url;
    config.signer.tokens.legacy_token = token;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    configTime(0, 0, "pool.ntp.org");
    return Firebase.ready();
}

bool sendAirQuality(int ppm)
{
    String logPath = "/ppm";
    return Firebase.RTDB.setInt(&fbdo, logPath.c_str(), ppm);
}

bool sendLog(String deviceID, String state)
{

    String timestamp = getISOTime();
    FirebaseJson json;
    json.set("device_id", deviceID);
    json.set("timestamp", timestamp);
    json.set("state", state);
    String logPath = "/log/" + timestamp;
    return Firebase.RTDB.setJSON(&fbdo, logPath.c_str(), &json);
}
