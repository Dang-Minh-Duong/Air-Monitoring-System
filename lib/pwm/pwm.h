#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>

void pwm_init(uint8_t gpio_num, uint8_t channel_index, uint8_t timer_index,
              uint32_t freq_hz, uint8_t duty_resolution_bit, uint32_t duty);

void pwm_set_duty(uint8_t channel_index, uint32_t duty, uint8_t duty_resolution_bit);

void pwm_start(uint8_t channel_index);

void pwm_stop(uint8_t channel_index);

#endif
