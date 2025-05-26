#ifndef UART_DRIVER_H
#define UART_DRIVER_H
#include "gpio_config.h"

void uart2_init();
void uart2_send(const char* msg);

#endif

