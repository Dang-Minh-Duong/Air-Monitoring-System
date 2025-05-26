#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void pwm_init(uint8_t timer_num, uint8_t channel_num, uint8_t resolution_bits, uint8_t gpio_num, uint32_t freq_hz);
void pwm_set_duty(uint8_t channel_num, uint32_t duty_value);

#endif // PWM_H