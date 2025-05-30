#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include <stdint.h>
#include "ADC.h"

void gpio_pwm_setup(uint8_t gpio_num, uint8_t signal_index);
void gpio_output_configure_pin(int gpio_num);
void adc_configure_pin(adc_unit_t adc, int rtc_pin);


#endif // GPIO_CONFIG_H