#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>

/**
 * @brief Controls motor speed based on threshold values.
 *
 * @param x               Input value for control decision.
 * @param t1              First threshold.
 * @param t2              Second threshold.
 * @param t3              Third threshold.
 * @param channel_num     PWM channel number (0–7).
 * @param resolution_bits PWM resolution (e.g., 8, 10, 13...).
 */
void motor_control(uint16_t x, uint16_t t1, uint16_t t2, uint8_t channel_num, uint8_t resolution_bits);

#endif // MOTOR_CONTROL_H