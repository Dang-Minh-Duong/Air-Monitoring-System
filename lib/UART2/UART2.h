#include <stdint.h>
#include <stdbool.h>
#include "esp_attr.h"

#ifndef UART2_H
#define UART2_H

void UART2_Config(int);
void IRAM_ATTR UART2_ISR(void *arg); 
uint8_t UART2_GetChar(void);
bool UART2_Available(void);

#endif