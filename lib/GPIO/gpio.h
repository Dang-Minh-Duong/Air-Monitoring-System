#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "ADC.h"
/**
 * @brief Configures a GPIO pin to output a PWM signal.
 * 
 * @param gpio_num The GPIO pin number to configure (e.g., 0–39).
 * @param signal_index The PWM signal index to assign (0 for HS_CH0, 1 for HS_CH1, etc.).
 */
void gpio_pwm_setup(uint8_t gpio_num, uint8_t signal_index);

/**
 * @brief Configures a GPIO pin as a digital push-pull output, suitable for driving LEDs or digital signals.
 * 
 * @param gpio_num The GPIO pin number to configure (e.g., 0–39).
 */
void gpio_led_setup(int gpio_num);

/**
 * @brief Configures an RTC IO pin for analog input via the RTC ADC path.
 * 
 * @param adc The ADC unit to use (typically ADC_UNIT_1 or ADC_UNIT_2).
 * @param rtc_pin The RTC IO pin index (0–39) to configure for analog input.
 */
void gpio_adc_setup(adc_unit_t adc, int rtc_pin);



#endif 