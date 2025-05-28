#include "pwm.h"
#include "pwm_address.h"

/**
 * @brief Initializes a PWM channel with specified parameters.
 *
 * @param timer_num        Timer number (0 to 3) for PWM generation.
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution in bits (e.g., 8, 10, 13...).
 * @param gpio_num         GPIO number to output the PWM signal.
 * @param freq_hz          PWM frequency in Hertz.
 */
void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz) {
    DPORT_PERIP_CLK_EN_REG |= (1u << 11);

    volatile uint32_t* timer_conf = &LEDC_HSTIMER_CONF_REG(timer_num);
    *timer_conf &= ~(0xF);
    *timer_conf |= (resolution_bits);

    unsigned int divider = 80000000 / (freq_hz * (1UL << resolution_bits));
    *timer_conf &= ~(((1u << 18) - 1) << 5);
    *timer_conf |= ((divider) << 13);

    LEDC_HSCH_CONF0_REG(channel_num) &= ~0x3;
    LEDC_HSCH_CONF0_REG(channel_num) |= (timer_num & 0x3);
    LEDC_HSCH_CONF0_REG(channel_num) |= (1u << 2);

    LEDC_HSCH_HPOINT_REG(channel_num) = 1;

    LEDC_HSCH_DUTY_REG(channel_num) &= ~(0xffffff);
    LEDC_HSCH_DUTY_REG(channel_num) = (20 << 4);

    GPIO_FUNC_OUT_SEL_CFG_REG(gpio_num) = 71 + channel_num;

    if (gpio_num < 32) {
        GPIO_ENABLE_W1TS_REG = (1u << gpio_num);
    } else {
        GPIO_ENABLE1_REG |= (1u << (gpio_num - 32));
    }

    volatile uint32_t* io_mux_reg = (volatile uint32_t*)gpio_io_mux_addr[gpio_num];
    *io_mux_reg &= ~(0b111 << 12);
    *io_mux_reg |= (2 << 12);

    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);
    *timer_conf &= ~(1u << 24);
}

/**
 * @brief Sets the duty cycle for a PWM channel using a raw duty value.
 *
 * @param channel_num  PWM channel number (0 to 7).
 * @param duty_value   Duty value (raw, not in percent). Max depends on resolution.
 */
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value) {
    LEDC_HSCH_DUTY_REG(channel_num) = (duty_value << 4);
    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);
}

/**
 * @brief Sets the duty cycle for a PWM channel using a percentage.
 *
 * @param channel_num      PWM channel number (0 to 7).
 * @param resolution_bits  PWM resolution in bits.
 * @param percent          Duty cycle as a percentage (0.0f to 100.0f).
 */
void pwm_set_duty_percent(uint8_t channel_num, uint8_t resolution_bits, float percent) {
    if (percent < 0.0f) percent = 0.0f;
    if (percent > 100.0f) percent = 100.0f;

    uint32_t max_duty = (1UL << resolution_bits) - 1;
    uint32_t duty_value = (uint32_t)((percent / 100.0f) * max_duty);

    pwm_set_duty(channel_num, duty_value);
}
