#ifndef WIFI_H
#define WIFI_H


#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "string.h"
#include "esp_log.h"
/**
 * @brief Initializes WiFi in station mode and connects to the specified network.
 *
 * This function performs the following:
 * - Initializes NVS (Non-Volatile Storage)
 * - Initializes the TCP/IP stack and default event loop
 * - Configures the WiFi driver in station mode
 * - Connects to the given WiFi network using the provided SSID and password
 * - Prints the assigned IP address upon successful connection
 *
 * @param ssid The SSID (name) of the WiFi network to connect to. Must be a null-terminated string.
 * @param pass The password for the WiFi network. Must be a null-terminated string.
 */
void wifi_init_sta(const char* ssid, const char* pass);

#endif