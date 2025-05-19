#include "UART0.h"
#include <stdint.h>
#include <stdbool.h>

#define UART0_BASE        0x3FF40000
#define UART0_FIFO_REG    (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART0_STATUS_REG  (*(volatile uint32_t *)(UART0_BASE + 0x1C))

void UART0_Send_Char(char c) {
    while (((UART0_STATUS_REG >> 16) & 0xFF) >= 126);  
    UART0_FIFO_REG = c;
}
