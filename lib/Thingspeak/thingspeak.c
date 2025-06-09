#include "thingspeak.h"
void send_to_thingspeak(const char *api_key, int adc_value)
{
    char url[128];
    snprintf(url, sizeof(url),
             "http://api.thingspeak.com/update?api_key=%s&field1=%d",
             api_key, adc_value);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI("THINGSPEAK", "HTTP GET status = %d, content_length = %lld",
         esp_http_client_get_status_code(client),
         esp_http_client_get_content_length(client));

    } else {
        ESP_LOGE("THINGSPEAK", "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}
