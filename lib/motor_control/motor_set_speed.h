#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

/**
 * @brief Smoothly change motor speed to target duty cycle.
 *
 * This function ramps the motor's duty cycle to the desired target value in steps,
 * where each step changes by step_percent and delays delay_ms milliseconds.
 * The current duty cycle is automatically read from hardware.
 *
 * @param pwm_channel     PWM channel number (0–7).
 * @param resolution_bits PWM resolution (e.g., 8, 10, 13).
 * @param target_percent  Desired duty cycle (0.0 to 100.0).
 * @param step_percent    Step size in percent (e.g., 1.0 for 1%).
 * @param delay_ms        Delay per step in milliseconds.
 */
void motor_set_speed_ramp(uint8_t pwm_channel,
                          uint8_t resolution_bits,
                          float target_percent,
                          float step_percent,
                          uint32_t delay_ms);

#endif // MOTOR_H
