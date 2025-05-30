#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>

/**
 * @brief Stops the DC motor by setting PWM to 0 and clearing direction pin.
 *
 * @param pwm_channel     PWM channel number (0–7).
 * @param resolution_bits PWM resolution in bits (e.g. 8, 10, 13).
 * @param gpio_dir_pin    GPIO number used for direction control.
 */
void motor_stop(uint8_t pwm_channel, uint8_t resolution_bits, uint8_t gpio_dir_pin);

#endif // MOTOR_CONTROL_H