#include "pwm.h"

// ===== Peripheral control register definitions =====
#define DPORT_PERIP_CLK_EN_REG       (*(volatile uint32_t*)0x3FF000C0)     // Peripheral clock enable
#define GPIO_ENABLE_W1TS_REG         (*(volatile uint32_t*)0x3FF44024)     // Enable GPIO output for GPIO < 32
#define GPIO_ENABLE1_REG             (*(volatile uint32_t*)0x3FF4402C)     // Enable GPIO output for GPIO >= 32
#define GPIO_FUNC_OUT_SEL_CFG_REG(n) (*(volatile uint32_t*)(0x3FF44530 + (n) * 0x4))   // GPIO output signal routing

// ===== IO_MUX register addresses for GPIO pins =====
static const uint32_t gpio_io_mux_addr[] = {
    [0]  = 0x3FF49044, [1]  = 0x3FF49088, [2]  = 0x3FF49040, [3]  = 0x3FF49084,
    [4]  = 0x3FF49048, [5]  = 0x3FF4906C, [12] = 0x3FF49034, [13] = 0x3FF49038,
    [14] = 0x3FF49030, [15] = 0x3FF4903C, [16] = 0x3FF4904C, [17] = 0x3FF49050,
    [18] = 0x3FF49070, [19] = 0x3FF49074, [21] = 0x3FF4907C, [22] = 0x3FF49080,
    [23] = 0x3FF4908C, [25] = 0x3FF49024, [26] = 0x3FF49028, [27] = 0x3FF4902C,
    [32] = 0x3FF4901C, [33] = 0x3FF49020, [34] = 0x3FF49014, [35] = 0x3FF49018,
    [36] = 0x3FF49004, [37] = 0x3FF49008, [38] = 0x3FF4900C, [39] = 0x3FF49010,
};

// ===== LEDC timer and channel base register definitions =====
#define LEDC_HSTIMER_CONF_REG(n) (*(volatile uint32_t*)(0x3FF59140 + (n) * 0x8))      // High-speed timer configuration
#define LEDC_HSCH_CONF0_REG(n)   (*(volatile uint32_t*)(0x3FF59000 + (n) * 0x14))     // Channel configuration 0
#define LEDC_HSCH_HPOINT_REG(n)  (*(volatile uint32_t*)(0x3FF59004 + (n) * 0x14))     // High point
#define LEDC_HSCH_DUTY_REG(n)    (*(volatile uint32_t*)(0x3FF59008 + (n) * 0x14))     // Duty cycle
#define LEDC_HSCH_CONF1_REG(n)   (*(volatile uint32_t*)(0x3FF5900C + (n) * 0x14))     // Channel configuration 1

// ===== PWM initialization =====
void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz) {
    // Enable LEDC module clock (bit 11 = LEDC_EN)
    DPORT_PERIP_CLK_EN_REG |= (1u << 11);

    // Configure LEDC high-speed timer
    volatile uint32_t* timer_conf = &LEDC_HSTIMER_CONF_REG(timer_num);

    *timer_conf &= ~(0xF);                      // Clear resolution bits [4:0]
    *timer_conf |= (resolution_bits);     // Set resolution (duty precision)

    // Compute clock divider: 80 MHz / (freq * 2^resolution)
    unsigned int divider = 80000000 / (freq_hz * (1UL << resolution_bits));

    *timer_conf &= ~(((1u << 18) - 1) << 5);;             // Clear divider bits [22:5]
    *timer_conf |= ((divider) << 13);  // Set divider

    

    // Configure LEDC channel
    LEDC_HSCH_CONF0_REG(channel_num) &= ~0x3;                // Clear timer select bits [1:0]
    LEDC_HSCH_CONF0_REG(channel_num) |= (timer_num & 0x3);   // Assign timer to channel

    LEDC_HSCH_CONF0_REG(channel_num) |= (1u << 2);           // Enable output for channel (bit 2)

    LEDC_HSCH_HPOINT_REG(channel_num) = 1;                   // Set high point to 0 (no delay)

    LEDC_HSCH_DUTY_REG(channel_num) &= ~(0xffffff);
    LEDC_HSCH_DUTY_REG(channel_num) = (20 << 4);              // Set initial duty cycle = 0
    

    // Route PWM signal to GPIO
    GPIO_FUNC_OUT_SEL_CFG_REG(gpio_num) = 71 + channel_num;  // 0x40 = PWM0 base output index

    // Enable GPIO as output
    if (gpio_num < 32) {
        GPIO_ENABLE_W1TS_REG = (1u << gpio_num);
    } else {
        GPIO_ENABLE1_REG |= (1u << (gpio_num - 32));
    }

    // Configure GPIO function via IO_MUX
    volatile uint32_t* io_mux_reg = (volatile uint32_t*)gpio_io_mux_addr[gpio_num];
    *io_mux_reg &= ~(0b111 << 12);               // Clear FUNC_SEL bits [14:12]
    *io_mux_reg |= (2 << 12);                    // Set function to FUNC2 = output

    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);          // Trigger duty update (bit 31)
    *timer_conf &= ~(1u << 24);                  // Clear pause bit to start timer
}
// ===== Update PWM duty cycle =====
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value) {
    LEDC_HSCH_DUTY_REG(channel_num) = (duty_value << 4);     // Set duty (shifted left 4 bits)
    LEDC_HSCH_CONF1_REG(channel_num) |= (1u << 31);          // Trigger update (bit 31)
}
void pwm_set_duty_percent(uint8_t channel_num, uint8_t resolution_bits, float percent) {
    if (percent < 0.0f) percent = 0.0f;
    if (percent > 100.0f) percent = 100.0f;

    uint32_t max_duty = (1UL << resolution_bits) - 1;  // 2^res - 1
    uint32_t duty_value = (uint32_t)((percent / 100.0f) * max_duty);

    pwm_set_duty(channel_num, duty_value);
}

