#include "gpio_config.h"
#include <stdint.h>

// ===== ESP32 GPIO Register Address Definitions =====
#define GPIO_ENABLE_W1TS_REG     (*(volatile uint32_t*)0x3FF44024)  // Output enable set (GPIO0~31)
#define GPIO_ENABLE1_W1TS_REG    (*(volatile uint32_t*)0x3FF4402C)  // Output enable set (GPIO32~39)

#define GPIO_OUT_W1TS_REG        (*(volatile uint32_t*)0x3FF44008)  // Set GPIO output high (0~31)
#define GPIO_OUT_W1TC_REG        (*(volatile uint32_t*)0x3FF4400C)  // Set GPIO output low (0~31)
#define GPIO_OUT1_W1TS_REG       (*(volatile uint32_t*)0x3FF44010)  // Set GPIO output high (32~39)
#define GPIO_OUT1_W1TC_REG       (*(volatile uint32_t*)0x3FF44014)  // Set GPIO output low (32~39)

// IO_MUX base address array for FUNC_SEL and config
static const uint32_t gpio_io_mux_addr[] = {
    0x3FF49044, 0x3FF49088, 0x3FF49040, 0x3FF49084, 0x3FF49048,
    0x3FF4906C, 0, 0, 0, 0, 0, 0, 0x3FF49034, 0x3FF49038, 0x3FF49030,
    0x3FF4903C, 0x3FF4904C, 0x3FF49050, 0x3FF49070, 0x3FF49074,
    0, 0x3FF4907C, 0x3FF49080, 0x3FF4908C,
    0, 0x3FF49024, 0x3FF49028, 0x3FF4902C
};

// ===== Hàm cấu hình GPIO output (thanh ghi) =====
static void gpio_config_output(gpio_num_t pin) {
    // 1. Enable output for pin
    if (pin < 32) {
        GPIO_ENABLE_W1TS_REG = (1 << pin);
    } else {
        GPIO_ENABLE1_W1TS_REG = (1 << (pin - 32));
    }

    // 2. Đặt FUNC_SEL = 0 (chế độ GPIO) qua IO_MUX
    if (pin < sizeof(gpio_io_mux_addr)/sizeof(uint32_t) && gpio_io_mux_addr[pin] != 0) {
        volatile uint32_t *reg = (volatile uint32_t*)gpio_io_mux_addr[pin];
        *reg &= ~(0x7 << 0); // FUNC_SEL = 0 (GPIO function)
    }
}

// ===== Hàm public: cấu hình chân LED làm output =====
void gpio_config_led_output(gpio_num_t pin) {
    gpio_config_output(pin);
}

// ===== Hàm public: cấu hình chân motor làm output =====
void gpio_config_motor_output(gpio_num_t pin) {
    gpio_config_output(pin);
}
