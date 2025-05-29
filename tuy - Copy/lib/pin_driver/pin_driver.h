#ifndef PIN_DRIVER_H
#define PIN_DRIVER_H

#include "driver/gpio.h"
#include <stdbool.h>

// Kiểu enum cho chế độ chân
typedef enum {
    PIN_MODE_INPUT = GPIO_MODE_INPUT,
    PIN_MODE_OUTPUT = GPIO_MODE_OUTPUT,
    PIN_MODE_INPUT_OUTPUT = GPIO_MODE_INPUT_OUTPUT,
} pin_mode_t;

// Kiểu enum cho trạng thái chân
typedef enum {
    PIN_LEVEL_LOW = 0,
    PIN_LEVEL_HIGH = 1
} pin_level_t;

// Hàm khởi tạo chân GPIO
void pin_init(gpio_num_t pin, pin_mode_t mode, bool pullup_en, bool pulldown_en);

// Ghi mức logic ra chân GPIO
void pin_write(gpio_num_t pin, pin_level_t level);

// Đọc mức logic từ chân GPIO
pin_level_t pin_read(gpio_num_t pin);

// Đảo trạng thái chân GPIO (chỉ áp dụng với chân output)
void pin_toggle(gpio_num_t pin);

#endif // PIN_DRIVER_H
