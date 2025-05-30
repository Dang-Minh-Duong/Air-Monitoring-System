#include "pwm.h"
#include "gpio_address.h"

/**
 * @brief Stops the DC motor by setting PWM to 0 and clearing direction pin.
 *
 * @param pwm_channel     PWM channel number (0–7).
 * @param resolution_bits PWM resolution in bits (e.g. 8, 10, 13).
 * @param gpio_dir_pin    GPIO number used for direction control.
 */
void motor_stop(uint8_t pwm_channel, uint8_t resolution_bits, uint8_t gpio_dir_pin) {
    // Set PWM duty cycle to 0%
    pwm_set_duty_percent(pwm_channel, resolution_bits, 0.0f);

    // Set direction pin to 0
    if (gpio_dir_pin < 32) {
        GPIO_ENABLE_W1TS_REG = (1u << gpio_dir_pin);  // Enable output
        GPIO_OUT_W1TC_REG = (1u << gpio_dir_pin);     // Set to 0
    } else {
        GPIO_ENABLE1_REG |= (1u << (gpio_dir_pin - 32));
        GPIO_OUT1_W1TC_REG = (1u << (gpio_dir_pin - 32));
    }
}
