#ifndef BLYNK_MODULE_H
#define BLYNK_MODULE_H
#include <stdint.h>
#include "esp_http_client.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include <motor_control.h>

/**
 * @brief Perform an HTTP GET request and optionally retrieve the response.
 *
 * This function initializes an HTTP client and performs a GET request to the specified URL.
 * If a response buffer is provided, the response content will be read and stored in the buffer.
 *
 * @param url The full HTTP URL to send the GET request to.
 * @param response_buf Pointer to a buffer to store the response. Can be NULL if response is not needed.
 * @param buf_size The size of the response buffer in bytes.
 * @param tag A tag used for logging (usually the calling module's name).
 *
 * @return
 * - ESP_OK on success
 * - ESP_FAIL or specific esp_err_t error code on failure
 */
static esp_err_t blynk_http_get(const char *url, char *response_buf, size_t buf_size, const char *tag);
/**
 * @brief Sends an integer value to a Blynk virtual pin using the provided token.
 *
 * This function constructs a Blynk API HTTP GET request to update a virtual pin's value.
 * It does not include the actual value in the request body—this is a stub or placeholder
 * and may require further implementation if you want to send values.
 *
 * @param value The integer value to be sent (currently unused in request).
 * @param token The Blynk authentication token.
 * @param virtual_pin The virtual pin identifier, e.g., "v0", "v1", etc.
 * @param tag A tag used for logging (e.g., module name).
 */
void blynk_send_int(int value, const char *token, const char *virtual_pin, const char *tag);

#endif