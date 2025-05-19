#include "UART2.h"
#include "UART0.h"
#include <Arduino.h>
void setup() {
    UART2_Config(9600);
}

void loop() {
    if (UART2_Available()) {
        uint8_t c = UART2_GetChar();
        UART0_Send_Char(c);
    }
}
