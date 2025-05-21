#include "Clock_bus_init.h"
#include "stm32f4xx.h"

/**
 * @brief  Enable clocks for GPIOA, ADC1, and TIM2 peripherals.
 *
 * This function enables the necessary peripheral clocks via RCC:
 * - GPIOA: Used for analog input and motor control pins.
 * - ADC1 : For analog-to-digital conversion on PA0.
 * - TIM2 : For generating PWM signal on PA1 (Channel 2).
 */
void clock_bus_init(void)
{
    /* Enable clock for GPIOA (used by PA0, PA1, PA3, PA4, PA5) */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Enable clock for ADC1 (used to read analog input on PA0) */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /* Enable clock for TIM2 (used to generate PWM output on PA1) */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}
