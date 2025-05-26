#include "uart_driver.h"
#include <string.h>
#include "driver/uart.h"

void uart2_init() {
    uart_config_t config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_2, &config);
    //uart_set_pin(UART_NUM_2, pinMap.uart1, pinMap.uart1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_2, 1024, 0, 0, NULL, 0);
}

void uart2_send(const char* msg) {
    uart_write_bytes(UART_NUM_2, msg, strlen(msg));
}
