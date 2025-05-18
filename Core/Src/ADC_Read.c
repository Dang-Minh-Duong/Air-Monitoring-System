#include "ADC_READ.h"
#include "stm32f4xx.h"

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
