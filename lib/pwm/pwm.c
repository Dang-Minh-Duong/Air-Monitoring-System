#include <stdint.h>

#define LEDC_BASE_ADDR      0x3FF59000
#define IO_MUX_BASE_ADDR    0x3FF49000

#define LEDC_TIMER_CONF(n)  (LEDC_BASE_ADDR + 0x000 + (n) * 0x14)
#define LEDC_TIMER_CONF1(n) (LEDC_BASE_ADDR + 0x004 + (n) * 0x14)

#define LEDC_CH_CONF0(n)    (LEDC_BASE_ADDR + 0x020 + (n) * 0x14)
#define LEDC_CH_HPOINT(n)   (LEDC_BASE_ADDR + 0x024 + (n) * 0x14)
#define LEDC_CH_DUTY(n)     (LEDC_BASE_ADDR + 0x028 + (n) * 0x14)
#define LEDC_CH_CONF1(n)    (LEDC_BASE_ADDR + 0x02C + (n) * 0x14)

#define IO_MUX_REG(gpio)    (IO_MUX_BASE_ADDR + 0x44 + ((gpio) * 4))

void pwm_config_gpio(uint8_t gpio_num, uint8_t func_index) {
    volatile uint32_t *reg = (uint32_t *)IO_MUX_REG(gpio_num);
    *reg &= ~(0b111 << 8);
    *reg |= (func_index << 8);  // FUNCx = 2/3 tùy GPIO
}

void pwm_config_timer(uint8_t timer_index, uint32_t freq_hz, uint8_t duty_resolution_bit) {
    uint32_t clk_apb = 80000000;
    uint32_t precision = 1 << duty_resolution_bit;
    uint32_t div = (clk_apb << 8) / (freq_hz * precision);  // Q8.8

    *(volatile uint32_t *)LEDC_TIMER_CONF(timer_index) =
        (1 << 29) | // reset
        (0 << 28) | // unpause
        (0 << 24) | // clk = APB
        (duty_resolution_bit << 5) |
        ((div >> 8) & 0x1F); // phần nguyên

    *(volatile uint32_t *)LEDC_TIMER_CONF1(timer_index) = div & 0xFF; // phần thập phân
}

void pwm_set_duty(uint8_t channel_index, uint32_t duty, uint8_t duty_resolution_bit) {
    duty = (duty << 4); // LEDC dùng Q0.8 fixed point => shift 4 bit

    *(volatile uint32_t *)LEDC_CH_HPOINT(channel_index) = 0;
    *(volatile uint32_t *)LEDC_CH_DUTY(channel_index) = duty;
}

void pwm_start(uint8_t channel_index) {
    *(volatile uint32_t *)LEDC_CH_CONF1(channel_index) = (1 << 0); // duty_start
    *(volatile uint32_t *)LEDC_CH_CONF0(channel_index) |= (1 << 8) | (1 << 12); // para_up + sig_out_en
}

void pwm_init(uint8_t gpio_num,
              uint8_t channel_index,
              uint8_t timer_index,
              uint32_t freq_hz,
              uint8_t duty_resolution_bit,
              uint32_t duty) {
    pwm_config_gpio(gpio_num, 2); // phần lớn PWM func = 2
    pwm_config_timer(timer_index, freq_hz, duty_resolution_bit);

    // Liên kết timer -> channel
    *(volatile uint32_t *)LEDC_CH_CONF0(channel_index) &=
        ~(0b11 << 30); // clear timer_sel
    *(volatile uint32_t *)LEDC_CH_CONF0(channel_index) |= (timer_index << 30);

    pwm_set_duty(channel_index, duty, duty_resolution_bit);
    pwm_start(channel_index);
}
