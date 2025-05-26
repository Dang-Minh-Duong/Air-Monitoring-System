#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

void motor_init();
void motor_set_speed(uint8_t duty);

#endif
