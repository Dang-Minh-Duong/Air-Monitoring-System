#include "motor_control.h"
#include "pwm.h"

/**
 * @brief Controls motor speed based on a sensor input using threshold values.
 *
 * This function adjusts the motor PWM duty cycle depending on the value of `x`
 * compared to the thresholds `t1`, `t2`, and `t3`. The duty cycle is set to:
 * - 0% if x < t1
 * - 50% if t1 ≤ x < t2
 * - 100% if t2 ≤ x < t3
 * - 100% if x ≥ t3
 *
 * @param x               The input value used to determine motor speed.
 * @param t1              First threshold value (for 0% duty).
 * @param t2              Second threshold value (for 50% duty).
 * @param t3              Third threshold value (for 100% duty).
 * @param channel_num     PWM channel number (0 to 7).
 * @param resolution_bits Resolution of the PWM signal in bits.
 */
void motor_control(uint16_t x, uint16_t t1, uint16_t t2, uint8_t channel_num, uint8_t resolution_bits) {
    /*create duty percent var*/
    float duty_percent = 0.0f;

    /*simple control*/
    if (x < t1) {
        duty_percent = 0.0f;         // Turn off motor
    } else if (x < t2) {
        duty_percent = 50.0f;        // Set 50% duty
    } else {
        duty_percent = 100.0f;       // Set max duty
    }

    pwm_set_duty_percent(channel_num, resolution_bits, duty_percent);
}
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
// void motor_set_speed_ramp(uint8_t pwm_channel,
//                           uint8_t resolution_bits,
//                           float target_percent,
//                           float step_percent,
//                           uint32_t delay_ms)
// {
//     if (step_percent <= 0.0f || step_percent > 100.0f) return; // invalid step

//     // Automatically get the current duty cycle from hardware
//     float current_percent = pwm_get_duty_percent(pwm_channel, resolution_bits);

//     float step = (target_percent > current_percent) ? step_percent : -step_percent;
//     float value = current_percent;

//     while (fabsf(value - target_percent) > step_percent) {
//         value += step;

//         // Clamp to valid range (0.0 - 100.0)
//         if (value < 0.0f) value = 0.0f;
//         if (value > 100.0f) value = 100.0f;

//         pwm_set_duty_percent(pwm_channel, resolution_bits, value);
//         esp_timer_delay_ms(delay_ms);
//     }

//     // Set to final exact target to avoid overshoot or undershoot
//     pwm_set_duty_percent(pwm_channel, resolution_bits, target_percent);
// }

