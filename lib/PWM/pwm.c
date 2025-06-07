#include "pwm.h"

/**
 * @brief Initializes a PWM channel with specified parameters.
 *
 * @param timer_num        Timer number (0 to 3) for PWM generation.
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution in bits (e.g., 8, 10, 13...).
 * @param gpio_num         GPIO number to output the PWM signal.
 * @param freq_hz          PWM frequency in Hertz.
 */

void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz)
{
    /* enable ledc clock*/
    DPORT_PERIP_CLK_EN_REG |= (1u << 11);

    /*get timer config regs*/
    volatile uint32_t *timer_conf = &LEDC_HSTIMER_CONF_REG(timer_num);

    /*clear res bits*/
    *timer_conf &= ~(0xF);
    /*config res*/
    *timer_conf |= (resolution_bits);

    /*calculate divider*/
    unsigned int divider = 80000000 / (freq_hz * (1UL << resolution_bits));
    /*clear div bits*/
    *timer_conf &= ~(((1u << 18) - 1) << 5);
    /*write div*/
    *timer_conf |= ((divider) << 13);

    /*clear timer_num bits on channelx reg*/
    LEDC_HSCH_CONF0_REG(channel_num) &= ~0x3;
    /*choose timer*/
    LEDC_HSCH_CONF0_REG(channel_num) |= (timer_num & 0x3);
    /*enable signal out*/
    LEDC_HSCH_CONF0_REG(channel_num) |= (1u << 2);

    /*hpoint = 1*/
    LEDC_HSCH_HPOINT_REG(channel_num) = 1;

    /*reset duty cycle bits*/
    LEDC_HSCH_DUTY_REG(channel_num) &= ~(0xffffff);
    /*set temp duty value*/
    LEDC_HSCH_DUTY_REG(channel_num) = (20 << 4);

    /*map to gpio */
    gpio_pwm_setup(gpio_num, channel_num);

    /*duty start*/
    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);

    /*rst timer*/
    *timer_conf &= ~(1u << 24);
}

/**
 * @brief Sets the duty cycle for a PWM channel using a raw duty value.
 *
 * @param channel_num  PWM channel number (0 to 7).
 * @param duty_value   Duty value (raw, not in percent). Max depends on resolution.
 */
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value)
{
    /*set duty*/
    LEDC_HSCH_DUTY_REG(channel_num) = (duty_value << 4);

    /*duty start*/
    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);
}

/**
 * @brief Sets the duty cycle for a PWM channel using a percentage.
 *
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution in bits.
 * @param percent          Duty cycle as a percentage (0.0f to 100.0f).
 */
void pwm_set_duty_percent(uint8_t channel_num, uint8_t resolution_bits, float percent)
{
    /*set percent boundary*/
    if (percent < 0.0f)
        percent = 0.0f;
    if (percent > 100.0f)
        percent = 100.0f;

    /*cal max duty*/
    uint32_t max_duty = (1UL << resolution_bits) - 1;

    /*cal duty value from percent and res*/
    uint32_t duty_value = (uint32_t)((percent / 100.0f) * max_duty);

    /*set duty value*/
    pwm_set_duty(channel_num, duty_value);
}
/**
 * @brief Retrieves the current duty cycle (as a percentage) for a PWM channel.
 *
 * This function reads the raw duty cycle value from the hardware register and converts
 * it into a percentage based on the specified resolution.
 *
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution in bits (e.g., 8, 10, 13...).
 * @return                 Current duty cycle in percentage (0.0f to 100.0f).
 */
float pwm_get_duty_percent(uint8_t channel_num, uint8_t resolution_bits)
{
    if (channel_num > 7 || resolution_bits == 0 || resolution_bits > 20)
        return 0.0f;

    // Read the duty register value (bits [24:4] hold the actual duty)
    uint32_t reg_val = LEDC_HSCH_DUTY_REG(channel_num);
    uint32_t duty_raw = reg_val >> 4; // Extract the actual duty value

    // Calculate maximum possible duty value based on resolution
    uint32_t max_duty = (1UL << resolution_bits) - 1;

    // Convert raw duty to percentage
    float percent = ((float)duty_raw / (float)max_duty) * 100.0f;

    return percent;
}