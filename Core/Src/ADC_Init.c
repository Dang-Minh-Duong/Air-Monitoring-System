#include "ADC_INIT.h"
#include "stm32f4xx.h"

/**
 * @brief  Initialize ADC1 to read analog input from channel 0 (PA0).
 *         - Sets ADC1 to single conversion mode.
 */
void ADC_Init(void)
{
    /* 1. Reset ADC1 configuration */
    ADC1->CR1 = 0;                           // No scan, no interrupt, 12-bit resolution
    ADC1->CR2 = 0;                           // Single conversion mode, right alignment

    /* 2. Configure regular channel sequence */
    ADC1->SQR1 = 0;                          // 1 conversion in the sequence
    ADC1->SQR3 = 0;                          // First conversion in regular sequence is channel 0 (PA0)

    /* 3. Enable ADC1 */
    ADC1->CR2 |= (1U << 0);              // Power on ADC1
}

/**
 * @brief  Start ADC1 conversion and return the converted 12-bit result.
 * @retval 12-bit ADC value (0 - 4095)
 */
uint16_t ADC_Read(void)
{
    /* 1. Start software conversion by setting SWSTART bit  */
    ADC1->CR2 |= (1U << 30);

    /* 2. Wait for End of Conversion (EOC) flag  */
    while (!(ADC1->SR & (1U << 1)));

    /* 3. Read 12-bit conversion result from data register */
    return (uint16_t)(ADC1->DR & 0x0FFF);
}
