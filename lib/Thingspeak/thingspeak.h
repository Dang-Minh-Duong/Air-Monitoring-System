#ifndef THINGSPEAK_H
#define THINGSPEAK_H
#include "esp_http_client.h"
#include "esp_log.h"
void send_to_thingspeak(const char *api_key, int adc_value);
#endif