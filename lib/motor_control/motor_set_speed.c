#include "pwm.h"
#include "esp_timer.h"
#include <math.h>  // for fabsf

/**
 * @brief Smoothly change motor speed to target duty cycle.
 *
 * This function ramps the motor's duty cycle to the desired target value in steps,
 * where each step changes by step_percent and delays delay_ms milliseconds.
 *
 * @param pwm_channel     PWM channel number (0–7).
 * @param resolution_bits PWM resolution (e.g., 8, 10, 13).
 * @param current_percent Current duty cycle (0.0 to 100.0).
 * @param target_percent  Desired duty cycle (0.0 to 100.0).
 * @param step_percent    Step size in percent (e.g., 1.0 for 1%).
 * @param delay_ms        Delay per step in milliseconds.
 */
void motor_set_speed_ramp(uint8_t pwm_channel,
                          uint8_t resolution_bits,
                          float current_percent,
                          float target_percent,
                          float step_percent,
                          uint32_t delay_ms)
{
    if (step_percent <= 0.0f || step_percent > 100.0f) return; // invalid step

    float step = (target_percent > current_percent) ? step_percent : -step_percent;
    float value = current_percent;

    while (fabsf(value - target_percent) > step_percent) {
        value += step;

        // Clamp to valid range (0.0 - 100.0)
        if (value < 0.0f) value = 0.0f;
        if (value > 100.0f) value = 100.0f;

        pwm_set_duty_percent(pwm_channel, resolution_bits, value);
        esp_timer_delay_ms(delay_ms);
    }

    // Set to final exact target to avoid overshoot or undershoot
    pwm_set_duty_percent(pwm_channel, resolution_bits, target_percent);
}
