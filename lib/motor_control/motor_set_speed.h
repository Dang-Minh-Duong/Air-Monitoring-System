#ifndef PWM_RAMP_H
#define PWM_RAMP_H

#include <stdint.h>

/**
 * @file pwm_ramp.h
 * @brief PWM motor speed ramping control
 */

/**
 * @brief Smoothly ramp motor speed to target duty cycle within specified time
 * 
 * This function gradually adjusts the PWM duty cycle from its current value
 * to the target value in linear steps, ensuring the total transition time
 * matches the specified duration.
 * 
 * @param pwm_channel PWM channel number (0-7)
 * @param resolution_bits PWM resolution in bits (e.g., 8, 10, 12)
 * @param target_percent Target duty cycle percentage (0.0 to 100.0)
 * @param step_percent Absolute step size for each adjustment (0.0 < step ≤ 100.0)
 * @param total_ramp_time_ms Total time for ramp in milliseconds
 * 
 * @note The function will:
 *       - Automatically determine ramp direction (up/down)
 *       - Calculate optimal step delays
 *       - Clamp values to 0-100% range
 *       - Guarantee final precise target value
 *       - Maintain minimum 1ms delay between steps
 */
void motor_set_speed_ramp(uint8_t pwm_channel,
                         uint8_t resolution_bits,
                         float target_percent,
                         float step_percent,
                         uint32_t total_ramp_time_ms);

#endif /* PWM_RAMP_H */