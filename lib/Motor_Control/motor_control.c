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