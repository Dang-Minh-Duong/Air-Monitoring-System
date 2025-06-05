#include "pwm.h"
#include "motor_set_speed.h"
#include "esp_timer.h"
#include <math.h>  // for fabsf()

/**
 * @brief Smoothly ramp motor speed to target duty cycle within specified time
 * 
 * @param pwm_channel PWM channel number (0-7) to control
 * @param resolution_bits PWM resolution in bits (e.g., 8, 10, 12)
 * @param target_percent Target duty cycle (0.0 to 100.0)
 * @param step_percent Step size for each adjustment (0.0 < step ≤ 100.0)
 * @param total_ramp_time_ms Total time for ramp in milliseconds
 */
void motor_set_speed_ramp(uint8_t pwm_channel,
                         uint8_t resolution_bits,
                         float target_percent,
                         float step_percent,
                         uint32_t total_ramp_time_ms)
{
    /* Validate step percentage */
    if (step_percent <= 0.0f || step_percent > 100.0f) return;

    /* Get current duty cycle and determine direction */
    float current_percent = pwm_get_duty_percent(pwm_channel, resolution_bits);
    float step = (target_percent > current_percent) ? step_percent : -step_percent;
    float value = current_percent;

    /* Calculate automatic timing */
    float percent_diff = fabsf(target_percent - current_percent);
    uint32_t num_steps = (uint32_t)(percent_diff / step_percent);
    
    /* Prevent division by zero when no ramp needed */
    uint32_t delay_ms = (num_steps > 0) ? (total_ramp_time_ms / num_steps) : 0;

    /* Ensure minimum delay for system stability */
    if (delay_ms < 1) delay_ms = 1;

    /* Execute ramp */
    while (fabsf(value - target_percent) > step_percent) {
        value += step;
        /* Clamp value to valid range */
        value = (value < 0.0f) ? 0.0f : (value > 100.0f) ? 100.0f : value;

        pwm_set_duty_percent(pwm_channel, resolution_bits, value);
        esp_timer_delay_ms(delay_ms);
    }

    /* Ensure precise final value */
    pwm_set_duty_percent(pwm_channel, resolution_bits, target_percent);
}