#include "wifi.h"

/**
 * @brief Initialize and connect to a WiFi Access Point in Station mode.
 * 
 * This function initializes the NVS flash, TCP/IP stack, event loop,
 * creates a default WiFi station interface, configures WiFi parameters
 * including SSID and password, then starts and connects the WiFi.
 * After connection, it prints the assigned IP address.
 * 
 * @param ssid Pointer to the SSID string of the WiFi network
 * @param pass Pointer to the password string of the WiFi network
 */
void wifi_init_sta(const char* ssid, const char* pass) {
    /* Initialize Non-Volatile Storage (NVS) - required for WiFi */
    ESP_ERROR_CHECK(nvs_flash_init());

    /* Initialize TCP/IP network interface */
    ESP_ERROR_CHECK(esp_netif_init());

    /* Create default event loop for handling system events */
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* Create default WiFi station interface */
    esp_netif_create_default_wifi_sta();

    /* Initialize WiFi driver with default configuration */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /* Prepare WiFi station configuration structure */
    wifi_config_t wifi_config = {
        .sta = {
            /* Set minimum authentication mode */
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            /* Protected Management Frames configuration */
            .pmf_cfg = {
                .capable = true,
                .required = false,
            },
        },
    };

    /* Copy SSID and password into config (with size protection) */
    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, pass, sizeof(wifi_config.sta.password));

    /* Set WiFi mode to Station */
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    /* Apply the WiFi configuration for station interface */
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    /* Start WiFi driver */
    ESP_ERROR_CHECK(esp_wifi_start());

    /* Initiate WiFi connection */
    ESP_ERROR_CHECK(esp_wifi_connect());

    /* Retrieve IP information after connection */
    esp_netif_ip_info_t ip_info;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    esp_netif_get_ip_info(netif, &ip_info);

    /* Print the acquired IP address */
    printf("Got IP: " IPSTR "\n", IP2STR(&ip_info.ip));
}
