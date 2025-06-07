#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/**
 * @brief Initialize a PWM channel with given parameters.
 *
 * This function configures the timer and channel registers for PWM output,
 * calculates the clock divider for the desired frequency and resolution,
 * and maps the PWM output to the specified GPIO pin.
 *
 * @param timer_num        Timer number (0 to 3) used for PWM generation.
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution in bits (e.g., 8, 10, 13).
 * @param gpio_num         GPIO number to output the PWM signal.
 * @param freq_hz          Desired PWM frequency in Hertz.
 */
void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz);

/**
 * @brief Set raw duty cycle value on a PWM channel.
 *
 * The duty value should be in the range based on PWM resolution.
 * The value is shifted left by 4 before writing to hardware register.
 *
 * @param channel_num  PWM channel number (0 to 7).
 * @param duty_value   Raw duty cycle value (not percentage).
 */
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value);

/**
 * @brief Set duty cycle for a PWM channel in percentage.
 *
 * Converts the percentage into a raw duty value based on resolution
 * and calls pwm_set_duty() to update the channel.
 *
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution bits (e.g., 8, 10, 13).
 * @param percent          Duty cycle percentage (0.0 to 100.0).
 */
void pwm_set_duty_percent(uint8_t channel_num, uint8_t resolution_bits, float percent);

/**
 * @brief Get the current duty cycle as a percentage for a PWM channel.
 *
 * Reads the raw duty cycle register, extracts the value, and converts it
 * to a percentage based on PWM resolution.
 *
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution bits.
 * @return                 Current duty cycle percentage (0.0 to 100.0).
 */
float pwm_get_duty_percent(uint8_t channel_num, uint8_t resolution_bits);

/**
 * @brief Categorize duty cycle percentage into discrete levels.
 *
 * Levels:
 *  - 0: duty == 0%
 *  - 1: duty > 0% and < 75%
 *  - 2: duty >= 75%
 *
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution bits.
 * @return                 Duty level (0, 1, or 2).
 */
uint8_t pwm_get_duty_level(uint8_t channel_num, uint8_t resolution_bits);

#endif // PWM_H
