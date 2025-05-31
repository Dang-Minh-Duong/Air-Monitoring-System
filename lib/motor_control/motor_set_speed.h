#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>

void motor_set_speed_ramp(uint8_t pwm_channel,
                          uint8_t resolution_bits,
                          float current_percent,
                          float target_percent,
                          float step_percent,
                          uint32_t delay_ms);

#endif // MOTOR_CONTROL_H
