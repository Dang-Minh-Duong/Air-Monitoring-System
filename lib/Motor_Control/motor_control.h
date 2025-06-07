#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>
#include "timer.h"
#include "pwm.h"
#include <math.h>
/**
 * @brief Set motor speed based on PPM value with threshold ranges
 * 
 * @param pwm_channel     PWM channel number (0-7)
 * @param resolution_bits PWM resolution (e.g., 8, 10, 13)
 * @param ppm_value      Current PPM reading
 * @param thres1         First threshold 
 * @param thres2         Second threshold 
 * @param ramp_step      Step size for ramping (percentage)
 * @param ramp_delay_ms  Delay between steps (milliseconds)
 */
void motor_control_auto(uint8_t pwm_channel,
                        uint8_t resolution_bits,
                        uint16_t value,
                        float thres1,
                        float thres2,
                        float ramp_step,
                        uint32_t total_ramp_time_ms, int groupTimer, int timer);
/**
 * @brief Smoothly ramp motor speed to target duty cycle within specified time
 * 
 * @param pwm_channel PWM channel number (0-7) to control
 * @param resolution_bits PWM resolution in bits (e.g., 8, 10, 12)
 * @param target_percent Target duty cycle (0.0 to 100.0)
 * @param step_percent Step size for each adjustment (0.0 < step ≤ 100.0)
 * @param total_ramp_time_ms Total time for ramp in milliseconds
 * @param groupTimer Group of timer
 * @param timer Timer index within the selected group (0 or 1).
 */
void motor_set_speed_ramp(uint8_t pwm_channel,
                          uint8_t resolution_bits,
                          float target_percent,
                          float step_percent,
                          uint32_t total_ramp_time_ms, int groupTimer, int timer);

#endif 