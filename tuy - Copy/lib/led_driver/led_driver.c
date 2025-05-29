#include "led_driver.h"

// Địa chỉ thanh ghi GPIO Output Enable và Output Register
#define GPIO_ENABLE_W1TS_REG     (*(volatile uint32_t *)0x3FF44024)  // Enable output (set bits)
#define GPIO_ENABLE_W1TC_REG     (*(volatile uint32_t *)0x3FF44028)  // Disable output (clear bits)
#define GPIO_OUT_W1TS_REG        (*(volatile uint32_t *)0x3FF44008)  // Set output HIGH
#define GPIO_OUT_W1TC_REG        (*(volatile uint32_t *)0x3FF4400C)  // Set output LOW
#define GPIO_IN_REG              (*(volatile uint32_t *)0x3FF4403C)  // Read input level

// Khởi tạo LED: cấu hình chân GPIO làm output
void led_init(const led_t* led) {
    if (led == NULL) return;
    if (led->pin < 0 || led->pin > 39) return;

    // Enable output cho chân GPIO led->pin
    GPIO_ENABLE_W1TS_REG = (1u << led->pin);
}

// Bật LED theo cấu hình active_level
void led_on(const led_t* led) {
    if (led == NULL) return;
    if (led->pin < 0 || led->pin > 39) return;

    if (led->active_level == LED_ACTIVE_HIGH) {
        GPIO_OUT_W1TS_REG = (1u << led->pin);  // Đặt chân HIGH để LED sáng
    } else {
        GPIO_OUT_W1TC_REG = (1u << led->pin);  // Đặt chân LOW để LED sáng
    }
}

// Tắt LED theo cấu hình active_level
void led_off(const led_t* led) {
    if (led == NULL) return;
    if (led->pin < 0 || led->pin > 39) return;

    if (led->active_level == LED_ACTIVE_HIGH) {
        GPIO_OUT_W1TC_REG = (1u << led->pin);  // Đặt chân LOW để LED tắt
    } else {
        GPIO_OUT_W1TS_REG = (1u << led->pin);  // Đặt chân HIGH để LED tắt
    }
}

// Đảo trạng thái LED (toggle)
void led_toggle(const led_t* led) {
    if (led == NULL) return;
    if (led->pin < 0 || led->pin > 39) return;

    // Đọc trạng thái hiện tại chân GPIO
    uint32_t level = GPIO_IN_REG & (1u << led->pin);

    if (level) { // Nếu hiện tại chân HIGH
        // Nếu active HIGH thì tắt LED (đặt LOW), ngược lại bật LED (đặt HIGH)
        if (led->active_level == LED_ACTIVE_HIGH) {
            GPIO_OUT_W1TC_REG = (1u << led->pin); // Tắt LED
        } else {
            GPIO_OUT_W1TS_REG = (1u << led->pin); // Bật LED
        }
    } else {    // Nếu hiện tại chân LOW
        if (led->active_level == LED_ACTIVE_HIGH) {
            GPIO_OUT_W1TS_REG = (1u << led->pin); // Bật LED
        } else {
            GPIO_OUT_W1TC_REG = (1u << led->pin); // Tắt LED
        }
    }
}
