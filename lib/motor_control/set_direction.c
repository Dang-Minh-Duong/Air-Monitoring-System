#include <stdint.h>
#include <stdbool.h>
#include "gpio_address.h"

/**
 * @brief Sets the direction of the DC motor.
 *
 * This function sets the specified GPIO pin to control motor direction.
 *
 * @param gpio_dir_pin   GPIO number used for direction control.
 * @param is_forward     true for forward direction, false for reverse.
 */
void motor_set_direction(uint8_t gpio_dir_pin, bool is_forward) {
    // Enable output for GPIO
    if (gpio_dir_pin < 32) {
        GPIO_ENABLE_W1TS_REG = (1u << gpio_dir_pin);
    } else {
        GPIO_ENABLE1_REG |= (1u << (gpio_dir_pin - 32));
    }

    // Set direction
    if (is_forward) {
        GPIO_OUT_W1TS_REG = (1u << gpio_dir_pin);   // set to 1
    } else {
        GPIO_OUT_W1TC_REG = (1u << gpio_dir_pin);   // clear to 0
    }
}
