#ifndef LED_H
#define LED_H
#include "gpio_address.h"
#include <stdint.h>
void ledControl(int led1, int led2, int led3, uint16_t value, uint16_t t1, uint16_t t2); 
#endif