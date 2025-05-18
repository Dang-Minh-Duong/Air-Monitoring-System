#include "ADC_INIT.h"
#include "stm32f4xx.h"

/**
 * @brief  Initialize ADC1 to read analog input from channel 0 (PA0).
 *         - Enables GPIOA and ADC1 clocks.
 *         - Configures PA0 as analog mode.
 *         - Sets ADC1 to single conversion mode.
 */
void ADC_Init(void)
{
    /* 1. Enable clocks for GPIOA and ADC1 */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;     // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;      // Enable ADC1 clock

    /* 2. Configure PA0 as analog input */
    GPIOA->MODER |= (3U << (0 * 2));         // MODER0[1:0] = 11: Analog mode
    GPIOA->PUPDR &= ~(3U << (0 * 2));        // No pull-up, no pull-down

    /* 3. Reset ADC1 configuration */
    ADC1->CR1 = 0;                           // No scan, no interrupt, 12-bit resolution
    ADC1->CR2 = 0;                           // Single conversion mode, right alignment

    /* 4. Configure regular channel sequence */
    ADC1->SQR1 = 0;                          // 1 conversion in the sequence
    ADC1->SQR3 = 0;                          // First conversion in regular sequence is channel 0 (PA0)

    /* 5. Enable ADC1 */
    ADC1->CR2 |= (1U << 0);              // Power on ADC1
}
