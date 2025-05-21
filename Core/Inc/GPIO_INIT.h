#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#include <stdint.h>   // For uint8_t

/**
 * @brief  Initialize all required GPIOA pins.
 *         - Calls ADC, PWM, and LED GPIO configuration functions.
 */
void GPIO_init(void);

/**
 * @brief  Configure PA0 as analog input for ADC channel 0.
 */
void GPIO_ADC_init(void);

/**
 * @brief  Configure:
 *         - PA1 as alternate function (TIM2_CH2) for PWM output
 *         - PA2 as digital output for motor direction control
 * @param  direction: Initial state of PA2 (motor direction pin)
 *         - 0: LOW (e.g., clockwise)
 *         - 1: HIGH (e.g., counterclockwise)
 */
void GPIO_PWM_init(uint8_t direction);

/**
 * @brief  Configure PA3, PA4, PA5 as digital outputs to control LEDs.
 */
void GPIO_LED_init(void);

#endif  // GPIO_INIT_H
