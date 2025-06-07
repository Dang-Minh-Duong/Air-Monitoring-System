#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>

/**
 * @brief Automatically control motor speed based on sensor value thresholds.
 *
 * @param pwm_channel PWM channel controlling the motor.
 * @param resolution_bits PWM resolution bits.
 * @param value Sensor value for threshold comparison.
 * @param thres1 Lower threshold.
 * @param thres2 Upper threshold.
 * @param ramp_step Step size for duty cycle ramping (percentage).
 * @param total_ramp_time_ms Total ramp duration in milliseconds.
 * @param groupTimer Timer group identifier for delay.
 * @param timer Timer identifier for delay.
 */
void motor_control_auto(uint8_t pwm_channel,
                        uint8_t resolution_bits,
                        uint16_t value,
                        float thres1,
                        float thres2,
                        float ramp_step,
                        uint32_t total_ramp_time_ms,
                        int groupTimer,
                        int timer);

/**
 * @brief Ramp motor PWM duty cycle smoothly to a target percentage.
 *
 * @param pwm_channel PWM channel controlling the motor.
 * @param resolution_bits PWM resolution bits.
 * @param target_percent Target duty cycle percentage (0-100%).
 * @param step_percent Increment/decrement step size (percentage).
 * @param total_ramp_time_ms Total time for the ramp in milliseconds.
 * @param groupTimer Timer group identifier for delay.
 * @param timer Timer identifier for delay.
 */
void motor_set_speed_ramp(uint8_t pwm_channel,
                          uint8_t resolution_bits,
                          float target_percent,
                          float step_percent,
                          uint32_t total_ramp_time_ms,
                          int groupTimer,
                          int timer);

#endif // MOTOR_CONTROL_H
