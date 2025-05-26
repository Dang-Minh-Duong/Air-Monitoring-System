#include "gpio_config.h"

PinMap pinMap;

void loadPinMap() {
    pinMap.motorPWM = 18;   // ví dụ
    pinMap.adcGas = 36;
    pinMap.ledRed = 2;
    pinMap.ledGreen = 4;
}
