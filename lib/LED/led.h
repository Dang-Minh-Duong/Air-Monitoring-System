#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>

/**
 * @brief Control three LEDs based on a value and two threshold levels.
 * 
 * This function turns on one LED and turns off the other two depending
 * on whether the value indicates good, normal, or bad air quality.
 * 
 * @param led1 GPIO number of the first LED (good air quality indicator).
 * @param led2 GPIO number of the second LED (normal air quality indicator).
 * @param led3 GPIO number of the third LED (bad air quality indicator).
 * @param value The value to compare with thresholds.
 * @param t1 Threshold 1 (lower threshold).
 * @param t2 Threshold 2 (upper threshold).
 */
void ledControl(int led1, int led2, int led3, uint16_t value, uint16_t t1, uint16_t t2);

#endif // LED_CONTROL_H
