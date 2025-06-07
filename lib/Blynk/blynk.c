#include "blynk.h"

/**
 * @brief Perform an HTTP GET request to the specified URL and optionally read the response.
 * 
 * @param url          The URL to send the GET request to.
 * @param response_buf Buffer to store the HTTP response (can be NULL if response is not needed).
 * @param buf_size     Size of the response buffer.
 * @param tag          Logging tag used for ESP_LOG.
 * @return esp_err_t   Returns ESP_OK on success, otherwise an error code.
 */
static esp_err_t blynk_http_get(const char *url, char *response_buf, size_t buf_size, const char *tag)
{
    // Configure the HTTP client with the specified URL and GET method
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
    };

    // Initialize the HTTP client
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL)
    {
        ESP_LOGE(tag, "Failed to initialize HTTP client");
        return ESP_FAIL;
    }

    // Perform the HTTP request
    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "HTTP GET failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);  // Clean up client resources
        return err;
    }

    // Check for HTTP status code 200 (OK)
    int status_code = esp_http_client_get_status_code(client);
    if (status_code != 200)
    {
        ESP_LOGE(tag, "Unexpected HTTP status code: %d", status_code);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    // If a response buffer is provided, read the response into it
    if (response_buf && buf_size > 0)
    {
        int len = esp_http_client_read_response(client, response_buf, buf_size - 1);
        if (len > 0)
        {
            response_buf[len] = '\0';  // Null-terminate the response string
            ESP_LOGW(tag, "Raw response: \"%s\"", response_buf);
        }
        else
        {
            ESP_LOGE(tag, "No response received (length = %d)", len);
            esp_http_client_cleanup(client);
            return ESP_FAIL;
        }
    }

    // Clean up the HTTP client resources
    esp_http_client_cleanup(client);
    return ESP_OK;
}

/**
 * @brief Send an integer value to a Blynk virtual pin via HTTP GET request.
 * 
 * @param value       The integer value to send.
 * @param token       The Blynk project authentication token.
 * @param virtual_pin The Blynk virtual pin to update (e.g. "V1").
 * @param tag         Logging tag used for ESP_LOG.
 */
void blynk_send_int(int value, const char *token, const char *virtual_pin, const char *tag)
{
    // Construct the Blynk API URL for sending the value
    // Format: http://blynk.cloud/external/api/get?token=TOKEN&Vx=VALUE
    char url[256];
    snprintf(url, sizeof(url),
         "http://blynk.cloud/external/api/get?token=%s&%s=",
         token, virtual_pin);

    // Send the HTTP GET request (without reading response)
    esp_err_t err = blynk_http_get(url, NULL, 0, tag);
    if (err == ESP_OK)
    {
        ESP_LOGI(tag, "Blynk update successful: %s = %d", virtual_pin, value);
    }
}
