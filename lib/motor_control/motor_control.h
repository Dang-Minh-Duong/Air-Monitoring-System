#ifndef MOTOR_SET_SPEED_H
#define MOTOR_SET_SPEED_H

#include <stdint.h>

/**
 * @brief Set motor speed based on PPM value with threshold ranges
 * 
 * @param pwm_channel     PWM channel number (0-7)
 * @param resolution_bits PWM resolution (e.g., 8, 10, 13)
 * @param ppm_value      Current PPM reading
 * @param thres1         First threshold (motor off below this)
 * @param thres2         Second threshold (25% duty between thres1 and thres2)
 * @param thres3         Third threshold (50% duty between thres2 and thres3)
 *                        (100% duty above thres3)
 * @param ramp_step      Step size for ramping (percentage)
 * @param ramp_delay_ms  Delay between steps (milliseconds)
 */
void motor_control(uint8_t pwm_channel,
                         uint8_t resolution_bits,
                         float ppm_value,
                         float thres1,
                         float thres2,
                         float thres3,
                         float ramp_step,
                         uint32_t ramp_delay_ms);

#endif /* MOTOR_SET_SPEED_H */