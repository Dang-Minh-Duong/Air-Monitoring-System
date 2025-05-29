#include "pin_driver.h"

// Địa chỉ thanh ghi GPIO Output Enable và Output Register
#define GPIO_ENABLE_W1TS_REG     (*(volatile uint32_t *)0x3FF44024)  // Enable output (set bits)
#define GPIO_ENABLE_W1TC_REG     (*(volatile uint32_t *)0x3FF44028)  // Disable output (clear bits)
#define GPIO_OUT_W1TS_REG        (*(volatile uint32_t *)0x3FF44008)  // Set output HIGH
#define GPIO_OUT_W1TC_REG        (*(volatile uint32_t *)0x3FF4400C)  // Set output LOW
#define GPIO_IN_REG              (*(volatile uint32_t *)0x3FF4403C)  // Read input level

// Địa chỉ thanh ghi kéo lên kéo xuống (Pull-up / Pull-down) GPIO (IO_MUX)
static const volatile uint32_t* GPIO_PIN_MUX_REGS[40] = {
    (uint32_t *)0x3FF49044,  // GPIO0
    (uint32_t *)0x3FF49088,  // GPIO1
    (uint32_t *)0x3FF49040,  // GPIO2
    (uint32_t *)0x3FF49084,  // GPIO3
    (uint32_t *)0x3FF49048,  // GPIO4
    (uint32_t *)0x3FF4906C,  // GPIO5
    NULL, NULL, NULL, NULL,  // GPIO6..9 (usually used for flash)
    NULL, NULL,
    (uint32_t *)0x3FF49034,  // GPIO12
    (uint32_t *)0x3FF49038,  // GPIO13
    (uint32_t *)0x3FF49030,  // GPIO14
    (uint32_t *)0x3FF4903C,  // GPIO15
    (uint32_t *)0x3FF4904C,  // GPIO16
    (uint32_t *)0x3FF49050,  // GPIO17
    (uint32_t *)0x3FF49070,  // GPIO18
    (uint32_t *)0x3FF49074,  // GPIO19
    NULL,
    (uint32_t *)0x3FF4907C,  // GPIO21
    (uint32_t *)0x3FF49080,  // GPIO22
    (uint32_t *)0x3FF4908C,  // GPIO23
    NULL, NULL,
    (uint32_t *)0x3FF49024,  // GPIO25
    (uint32_t *)0x3FF49028,  // GPIO26
    (uint32_t *)0x3FF4902C,  // GPIO27
    NULL, NULL, NULL,
    (uint32_t *)0x3FF4901C,  // GPIO32
    (uint32_t *)0x3FF49020,  // GPIO33
    (uint32_t *)0x3FF49014,  // GPIO34 (input only)
    (uint32_t *)0x3FF49018,  // GPIO35 (input only)
    (uint32_t *)0x3FF49004,  // GPIO36 (input only)
    (uint32_t *)0x3FF49008,  // GPIO37 (input only)
    (uint32_t *)0x3FF4900C,  // GPIO38 (input only)
    (uint32_t *)0x3FF49010   // GPIO39 (input only)
};

// Hàm cấu hình chế độ chân GPIO (input, output, input_output)
void pin_init(gpio_num_t pin, pin_mode_t mode, bool pullup_en, bool pulldown_en) {
    if (pin < 0 || pin > 39) return;

    // 1. Cấu hình IO_MUX để enable pull-up / pull-down
    volatile uint32_t* mux_reg = (uint32_t*)GPIO_PIN_MUX_REGS[pin];
    if (mux_reg != NULL) {
        uint32_t val = *mux_reg;
        // Clear pullup/pulldown bit (bit 9 pullup, bit 10 pulldown)
        val &= ~((1 << 9) | (1 << 10));
        if (pullup_en) val |= (1 << 9);
        if (pulldown_en) val |= (1 << 10);
        *mux_reg = val;
    }

    // 2. Enable hoặc disable output cho chân
    if (mode == PIN_MODE_OUTPUT || mode == PIN_MODE_INPUT_OUTPUT) {
        GPIO_ENABLE_W1TS_REG = (1u << pin);   // Enable output
    } else {
        GPIO_ENABLE_W1TC_REG = (1u << pin);   // Disable output (input mode)
    }
}

// Hàm viết mức logic ra chân GPIO
void pin_write(gpio_num_t pin, pin_level_t level) {
    if (pin < 0 || pin > 39) return;

    if (level == PIN_LEVEL_HIGH) {
        GPIO_OUT_W1TS_REG = (1u << pin);  // Set HIGH
    } else {
        GPIO_OUT_W1TC_REG = (1u << pin);  // Set LOW
    }
}

// Hàm đọc mức logic từ chân GPIO
pin_level_t pin_read(gpio_num_t pin) {
    if (pin < 0 || pin > 39) return PIN_LEVEL_LOW;

    uint32_t level = GPIO_IN_REG & (1u << pin);
    return (level) ? PIN_LEVEL_HIGH : PIN_LEVEL_LOW;
}

// Hàm đảo trạng thái chân GPIO (chỉ áp dụng cho output)
void pin_toggle(gpio_num_t pin) {
    if (pin < 0 || pin > 39) return;

    // Đọc mức hiện tại
    if (pin_read(pin) == PIN_LEVEL_HIGH) {
        pin_write(pin, PIN_LEVEL_LOW);
    } else {
        pin_write(pin, PIN_LEVEL_HIGH);
    }
}
