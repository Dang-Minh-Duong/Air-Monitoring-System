#include "motor_set_speed.h"
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
                         uint32_t ramp_delay_ms)
{
    float target_duty = 0.0f;
    
    // Determine target duty based on PPM thresholds
    if (ppm_value <= thres1) {
        target_duty = 0.0f;      // Motor off
    } 
    else if (ppm_value > thres1 && ppm_value <= thres2) {
        target_duty = 25.0f;     // 25% duty
    }
    else if (ppm_value > thres2 && ppm_value <= thres3) {
        target_duty = 50.0f;     // 50% duty
    }
    else {
        target_duty = 100.0f;    // 100% duty
    }
    
    // Use your existing ramp function for smooth transition
    motor_set_speed_ramp(pwm_channel, resolution_bits, target_duty, ramp_step, ramp_delay_ms);
}