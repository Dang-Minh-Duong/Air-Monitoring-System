#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

typedef struct {
    int motorPWM;
    int adcGas;
    int ledRed;
    int ledGreen;
} PinMap;

extern PinMap pinMap;
void loadPinMap();

#endif
