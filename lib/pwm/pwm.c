#include "pwm.h"

// Địa chỉ base LEDC (ESP32)
#define LEDC_BASE           0x3FF59000

// Các thanh ghi LEDC Timer (timer 0..3)
#define LEDC_TIMER_CONF_REG(timer_num) (*(volatile uint32_t*)(LEDC_BASE + 0x40 + (timer_num)*0x20))
#define LEDC_TIMER_DUTY_REG(timer_num) (*(volatile uint32_t*)(LEDC_BASE + 0x44 + (timer_num)*0x20))

// Các thanh ghi LEDC Channel (channel 0..7)
#define LEDC_CHANNEL_CONF_REG(ch_num) (*(volatile uint32_t*)(LEDC_BASE + 0x90 + (ch_num)*0x10))
#define LEDC_CHANNEL_DUTY_REG(ch_num) (*(volatile uint32_t*)(LEDC_BASE + 0x94 + (ch_num)*0x10))

// Địa chỉ GPIO MUX (bạn cần chỉnh sửa đúng theo datasheet)
#define GPIO_MUX_REG(gpio_num) (*(volatile uint32_t*)(0x3FF49000 + (gpio_num)*4))

#define TIMER_ENABLE_BIT    (1 << 31)
#define CHANNEL_ENABLE_BIT  (1 << 31)

void pwm_init(const pwm_config_t *cfg) {
    const uint32_t APB_CLK = 80000000;
    uint32_t prescaler = APB_CLK / (cfg->freq_hz * (1 << cfg->bit_num));
    if (prescaler == 0) prescaler = 1;

    // Cấu hình timer
    uint32_t timer_conf = 0;
    timer_conf |= (prescaler & 0xFF) << 0;
    timer_conf |= (cfg->bit_num & 0x1F) << 8;
    timer_conf |= TIMER_ENABLE_BIT;
    LEDC_TIMER_CONF_REG(cfg->timer_num) = timer_conf;

    // Cấu hình GPIO MUX cho chân gpio_num xuất PWM
    GPIO_MUX_REG(cfg->gpio_num) = 0x2;  // Thay đổi nếu cần

    // Cấu hình channel
    uint32_t channel_conf = 0;
    channel_conf |= (cfg->timer_num & 0x3) << 0;
    channel_conf |= CHANNEL_ENABLE_BIT;
    LEDC_CHANNEL_CONF_REG(cfg->channel_num) = channel_conf;

    // Thiết lập duty cycle
    uint32_t max_duty = (1 << cfg->bit_num) - 1;
    uint32_t duty_val = max_duty * cfg->duty_percent / 100;
    LEDC_CHANNEL_DUTY_REG(cfg->channel_num) = duty_val;
}
