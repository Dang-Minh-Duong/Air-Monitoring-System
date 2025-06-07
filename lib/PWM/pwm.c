#include "pwm.h"
#include "pwm_address.h"
#include "gpio_address.h"
#include "gpio_config.h"
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
void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz)
{
    /* Enable LEDC peripheral clock */
    DPORT_PERIP_CLK_EN_REG |= (1u << 11);

    /* Pointer to timer config register */
    volatile uint32_t *timer_conf = &LEDC_HSTIMER_CONF_REG(timer_num);

    /* Clear resolution bits */
    *timer_conf &= ~(0xF);
    /* Set resolution */
    *timer_conf |= (resolution_bits);

    /* Calculate clock divider for desired frequency and resolution */
    unsigned int divider = 80000000 / (freq_hz * (1UL << resolution_bits));

    /* Clear previous divider bits */
    *timer_conf &= ~(((1u << 18) - 1) << 5);
    /* Set new divider */
    *timer_conf |= ((divider) << 13);

    /* Clear timer select bits on channel config register */
    LEDC_HSCH_CONF0_REG(channel_num) &= ~0x3;
    /* Select timer for the channel */
    LEDC_HSCH_CONF0_REG(channel_num) |= (timer_num & 0x3);

    /* Enable PWM output signal on channel */
    LEDC_HSCH_CONF0_REG(channel_num) |= (1u << 2);

    /* Set hpoint to 1 (start point of PWM duty) */
    LEDC_HSCH_HPOINT_REG(channel_num) = 1;

    /* Clear previous duty cycle */
    LEDC_HSCH_DUTY_REG(channel_num) &= ~(0xffffff);
    /* Set initial duty value (scaled by 16) */
    LEDC_HSCH_DUTY_REG(channel_num) = (20 << 4);

    /* Configure GPIO pin for PWM output */
    gpio_pwm_setup(gpio_num, channel_num);

    /* Start duty cycle update */
    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);

    /* Reset timer */
    *timer_conf &= ~(1u << 24);
}

/**
 * @brief Set raw duty cycle value on a PWM channel.
 *
 * The duty value should be in the range based on PWM resolution.
 * The value is shifted left by 4 before writing to hardware register.
 *
 * @param channel_num  PWM channel number (0 to 7).
 * @param duty_value   Raw duty cycle value (not percentage).
 */
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value)
{
    /* Write duty value shifted left by 4 */
    LEDC_HSCH_DUTY_REG(channel_num) = (duty_value << 4);

    /* Trigger duty update */
    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);
}

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
void pwm_set_duty_percent(uint8_t channel_num, uint8_t resolution_bits, float percent)
{
    /* Clamp percent between 0 and 100 */
    if (percent < 0.0f)
        percent = 0.0f;
    if (percent > 100.0f)
        percent = 100.0f;

    /* Calculate maximum duty value based on resolution */
    uint32_t max_duty = (1UL << resolution_bits) - 1;

    /* Convert percentage to duty value */
    uint32_t duty_value = (uint32_t)((percent / 100.0f) * max_duty);

    /* Set duty */
    pwm_set_duty(channel_num, duty_value);
}

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
float pwm_get_duty_percent(uint8_t channel_num, uint8_t resolution_bits)
{
    /* Validate inputs */
    if (channel_num > 7 || resolution_bits == 0 || resolution_bits > 20)
        return 0.0f;

    /* Read raw duty register (bits 24:4) */
    uint32_t reg_val = LEDC_HSCH_DUTY_REG(channel_num);
    uint32_t duty_raw = reg_val >> 4;

    /* Calculate max duty based on resolution */
    uint32_t max_duty = (1UL << resolution_bits) - 1;

    /* Convert raw duty to percentage */
    float percent = ((float)duty_raw / (float)max_duty) * 100.0f;

    return percent;
}

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
uint8_t pwm_get_duty_level(uint8_t channel_num, uint8_t resolution_bits)
{
    float percent = pwm_get_duty_percent(channel_num, resolution_bits);

    if (percent == 0.0f)
        return 0;
    else if (percent < 75.0f)
        return 1;
    else
        return 2;
}
