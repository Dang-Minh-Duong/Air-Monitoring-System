#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>

/**
 * @file    motor_control.h
 * @brief   Motor control interface using GPIO and PWM based on ADC input.
 *
 * This header defines the interface for controlling a motor's direction and speed.
 * Direction is controlled via GPIO pins PA3, PA4, PA5.
 * Speed is controlled via PWM on TIM2 Channel 2 (CCR2).
 */

/**
 * @brief  Controls motor direction and speed using ADC input and threshold values.
 *
 * Logic:
 *   - adc_value < threshold1:    Forward (PA3 = 1), Low speed (PWM = 250)
 *   - threshold1 <= adc_value < threshold2: Neutral/Stop (PA4 = 1), Medium speed (PWM = 500)
 *   - adc_value >= threshold2:   Reverse (PA5 = 1), High speed (PWM = 750)
 *
 * @param adc_value   12-bit ADC input value (range: 0–4095)
 * @param threshold1  Lower threshold value (usually ~1/3 of 4095)
 * @param threshold2  Upper threshold value (usually ~2/3 of 4095)
 */
void motor_control(uint16_t adc_value, uint16_t threshold1, uint16_t threshold2);

#endif /* MOTOR_CONTROL_H */
