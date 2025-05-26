#include "pwm.h"

// ===== Peripheral control registers =====
#define DPORT_PERIP_CLK_EN_REG       (*(volatile uint32_t*)0x3FF000C0)     // Enable peripheral clocks
#define GPIO_ENABLE_W1TS_REG         (*(volatile uint32_t*)0x3FF44024)     // Enable output for GPIO < 32
#define GPIO_ENABLE1_REG             (*(volatile uint32_t*)0x3FF4402C)     // Enable output for GPIO >= 32
#define GPIO_FUNC0_OUT_SEL_CFG_REG   ((volatile uint32_t*)0x3FF44530)      // GPIO output signal selection

// ===== IO_MUX register addresses for each GPIO =====
static const uint32_t gpio_io_mux_addr[] = {
    [0]  = 0x3FF49044, [1]  = 0x3FF49088, [2]  = 0x3FF49040, [3]  = 0x3FF49084,
    [4]  = 0x3FF49048, [5]  = 0x3FF4906C, [12] = 0x3FF49034, [13] = 0x3FF49038,
    [14] = 0x3FF49030, [15] = 0x3FF4903C, [16] = 0x3FF4904C, [17] = 0x3FF49050,
    [18] = 0x3FF49070, [19] = 0x3FF49074, [21] = 0x3FF4907C, [22] = 0x3FF49080,
    [23] = 0x3FF4908C, [25] = 0x3FF49024, [26] = 0x3FF49028, [27] = 0x3FF4902C,
    [32] = 0x3FF4901C, [33] = 0x3FF49020, [34] = 0x3FF49014, [35] = 0x3FF49018,
    [36] = 0x3FF49004, [37] = 0x3FF49008, [38] = 0x3FF4900C, [39] = 0x3FF49010,
};

// ===== LEDC timer and channel register base =====
#define LEDC_HSTIMER_CONF_REG(n)     (*(volatile uint32_t*)(0x3FF59140 + (n) * 0x8))    // High-speed timer configuration
#define LEDC_HSCH_CONF0_REG(n)       (*(volatile uint32_t*)(0x3FF59000 + (n) * 0x14))   // Channel configuration 0
#define LEDC_HSCH_HPOINT_REG(n)      (*(volatile uint32_t*)(0x3FF59004 + (n) * 0x14))   // High point
#define LEDC_HSCH_DUTY_REG(n)        (*(volatile uint32_t*)(0x3FF59008 + (n) * 0x14))   // PWM duty
#define LEDC_HSCH_CONF1_REG(n)       (*(volatile uint32_t*)(0x3FF5900C + (n) * 0x14))   // Channel configuration 1

// ===== Initialize PWM =====
void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz) {
    // --- Enable LEDC peripheral clock ---
    DPORT_PERIP_CLK_EN_REG |= (1 << 11); // Bit 11: LEDC_EN

    // --- Configure LEDC timer ---
    volatile uint32_t* timer_conf = &LEDC_HSTIMER_CONF_REG(timer_num);

    *timer_conf &= ~(0x1F);                      // Clear previous resolution bits [0:4]
    *timer_conf |= (resolution_bits & 0x1F);     // Set new resolution (max 0x1F)

    // Calculate the clock divider: 80MHz / (frequency * 2^resolution)
    uint32_t divider = 80000000 / (freq_hz * (1 << resolution_bits));

    *timer_conf &= ~(0x3FFFF << 5);              // Clear divider bits [22:5]
    *timer_conf |= (divider << 13);              // Set divider (bits [22:5]), shifted appropriately

    // --- Configure PWM channel ---
    LEDC_HSCH_CONF0_REG(channel_num) &= ~(0b11);             // Clear old timer selection
    LEDC_HSCH_CONF0_REG(channel_num) |= (timer_num & 0x3);   // Select timer (0~3)

    LEDC_HSCH_CONF0_REG(channel_num) |= (1 << 2);            // Enable channel (bit 2 = EN)

    LEDC_HSCH_HPOINT_REG(channel_num) = 1;                   // Set high point to 1

    LEDC_HSCH_DUTY_REG(channel_num) &= ~(0xFFFFFF);          // Clear previous duty
    LEDC_HSCH_DUTY_REG(channel_num) = (20 << 4);             // Set default duty (shifted left 4 bits)

    GPIO_FUNC0_OUT_SEL_CFG_REG[gpio_num] = 71 + channel_num; // Route LEDC HS_CHx to GPIO

    if (gpio_num < 32) {
        GPIO_ENABLE_W1TS_REG |= (1 << gpio_num);             // Enable output for GPIO < 32
    } else {
        GPIO_ENABLE1_REG |= (1 << (gpio_num - 32));          // Enable output for GPIO ≥ 32
    }

    // --- Configure IO_MUX for selected GPIO ---
    volatile uint32_t* io_mux_reg = (volatile uint32_t*)gpio_io_mux_addr[gpio_num];
    *io_mux_reg &= ~(0b111 << 12);                           // Clear FUNC field
    *io_mux_reg |= (2 << 12);                                // Set FUNC2 (LEDC high-speed function)

    LEDC_HSCH_CONF1_REG(channel_num) |= (1 << 31);           // Trigger duty update

    // --- Unpause the timer (start it) ---
    *timer_conf &= ~(1 << 23); // Bit 23: PAUSE (write 0 to run)
}

// ===== Update PWM duty cycle at runtime =====
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value) {
    LEDC_HSCH_DUTY_REG(channel_num) = (duty_value << 4);     // Set new duty (shifted left 4 bits)
    LEDC_HSCH_CONF1_REG(channel_num) |= (1 << 31);           // Trigger duty update
}
