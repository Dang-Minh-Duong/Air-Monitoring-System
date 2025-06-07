#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* Macro for register write */
#ifndef REG_WRITE
#define REG_WRITE(reg, val) (*(volatile uint32_t *)(reg) = (val))
#endif

#ifndef BIT
#define BIT(n) (1U << (n))
#endif

/* ADC unit type enum - replace or include your ADC header if already defined */
typedef enum {
    ADC_UNIT_1 = 1,
    ADC_UNIT_2 = 2,
} adc_unit_t;

/**
 * @brief Configure a GPIO pin to output a PWM signal.
 * 
 * @param gpio_num GPIO number to configure.
 * @param channel_index PWM channel index offset for signal mapping.
 */
void gpio_pwm_setup(uint8_t gpio_num, uint8_t channel_index);

/**
 * @brief Configure an RTC IO pin for analog input through RTC ADC path.
 * 
 * @param adc ADC unit (ADC1 or ADC2).
 * @param rtc_pin RTC pin number to configure.
 */
void gpio_adc_setup(adc_unit_t adc, int rtc_pin);

/**
 * @brief Configure a GPIO pin as push-pull output for LED control.
 * 
 * @param gpio_num GPIO number to configure.
 */
void gpio_led_setup(int gpio_num);

/**
 * @brief Configure GPIO pins for I2C communication (SDA and SCL).
 * 
 * @param sda GPIO number for I2C SDA line.
 * @param scl GPIO number for I2C SCL line.
 */
void gpio_i2c_setup(int sda, int scl);

#endif /* GPIO_H */
