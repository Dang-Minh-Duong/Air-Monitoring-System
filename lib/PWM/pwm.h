#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "pwm_address.h"
#include "gpio.h"
/**
 * @brief Initializes a PWM channel with specified configuration.
 *
 * This function sets up a PWM output on the specified GPIO using the given timer, channel,
 * resolution, and frequency.
 *
 * @param timer_num        Timer number (0 to 3) for PWM generation.
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  Resolution of PWM in bits (e.g., 8, 10, 13...).
 * @param gpio_num         GPIO number to output the PWM signal.
 * @param freq_hz          Frequency of the PWM signal in Hertz.
 */
void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz);

/**
 * @brief Sets the PWM duty cycle using a raw value.
 *
 * This function sets the duty cycle for the specified PWM channel by directly
 * providing a duty value, which depends on the resolution.
 *
 * @param channel_num  PWM channel number (0 to 7).
 * @param duty_value   Duty value (0 to 2^resolution - 1).
 */
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value);

/**
 * @brief Sets the PWM duty cycle as a percentage.
 *
 * This function calculates the appropriate duty value based on the given percentage
 * and sets the duty cycle for the specified PWM channel.
 *
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution in bits.
 * @param percent          Duty cycle percentage (0.0f to 100.0f).
 */
void pwm_set_duty_percent(uint8_t channel_num, uint8_t resolution_bits, float percent);

#endif // PWM_H