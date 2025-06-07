#include "blynk.h"

static esp_err_t blynk_http_get(const char *url, char *response_buf, size_t buf_size, const char *tag)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL)
    {
        ESP_LOGE(tag, "Failed to init HTTP client");
        return ESP_FAIL;
    }

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "HTTP GET failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return err;
    }

    int status_code = esp_http_client_get_status_code(client);
    if (status_code != 200)
    {
        ESP_LOGE(tag, "Unexpected HTTP status: %d", status_code);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    if (response_buf && buf_size > 0)
    {
        int len = esp_http_client_read_response(client, response_buf, buf_size - 1);
        if (len > 0)
        {
            response_buf[len] = '\0';
            ESP_LOGW(tag, "Raw response: \"%s\"", response_buf);
        }
        else
        {
            ESP_LOGE(tag, "No response (length = %d)", len);
            esp_http_client_cleanup(client);
            return ESP_FAIL;
        }
    }

    esp_http_client_cleanup(client);
    return ESP_OK;
}

void blynk_send_int(int value, const char *token, const char *virtual_pin, const char *tag)
{
    char url[256];
    snprintf(url, sizeof(url),
         "http://blynk.cloud/external/api/get?token=%s&%s=",
         token, virtual_pin);

    esp_err_t err = blynk_http_get(url, NULL, 0, tag);
    if (err == ESP_OK)
    {
        ESP_LOGI(tag, "Blynk update successful: %s = %d", virtual_pin, value);
    }
}
