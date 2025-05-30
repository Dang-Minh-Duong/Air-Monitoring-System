#ifndef MOTOR_DIRECTION_H
#define MOTOR_DIRECTION_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Sets the direction of the DC motor.
 *
 * This function sets the specified GPIO pin to control motor direction.
 *
 * @param gpio_dir_pin   GPIO number used for direction control.
 * @param is_forward     true for forward direction, false for reverse.
 */
void motor_set_direction(uint8_t gpio_dir_pin, bool is_forward);

#endif // MOTOR_DIRECTION_H
