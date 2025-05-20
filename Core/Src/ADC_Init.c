#include "ADC_INIT.h"
#include "stm32f4xx.h"

/**
 * @brief ADC conversion result storage (12-bit max)
 */
volatile uint16_t adc_result = 0;

/**
 * @brief Initialize ADC1 with configurable resolution and sample time for channel 0 (PA0).
 *
 * @param resolution ADC resolution setting:
 *        0: 12-bit (default)
 *        1: 10-bit
 *        2: 8-bit
 *        3: 6-bit
 *
 * @param sample_time Sample time for channel 0 (PA0), valid values 0..7:
 *        0: 3 cycles
 *        1: 15 cycles
 *        2: 28 cycles
 *        3: 56 cycles
 *        4: 84 cycles
 *        5: 112 cycles
 *        6: 144 cycles
 *        7: 480 cycles
 */
void ADC_Init(uint8_t resolution, uint8_t sample_time)
{
    /* Enable ADC1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /* Reset ADC status register */
    ADC1->SR = 0;

    /* Configure ADC resolution */
    ADC1->CR1 &= ~ADC_CR1_RES;            // Clear resolution bits (bits 24,25)
    ADC1->CR1 |= ((resolution & 0x3) << 24);

    /* Configure sample time for channel 0 in SMPR2 register (bits 2:0) */
    ADC1->SMPR2 &= ~(7 << (3 * 0));       // Clear sample time bits for channel 0
    ADC1->SMPR2 |= ((sample_time & 0x7) << (3 * 0));

    /* Set regular sequence length to 1 conversion */
    ADC1->SQR1 &= ~ADC_SQR1_L;            // L = 0 means 1 conversion

    /* Set first conversion in regular sequence to channel 0 */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 0;                      // Channel 0

    /* Enable end-of-conversion (EOC) interrupt */
    ADC1->CR1 |= ADC_CR1_EOCIE;

    /* Disable continuous conversion mode (single conversion mode) */
    ADC1->CR2 &= ~ADC_CR2_CONT;

    /* Right data alignment (default) */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* Enable ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* Enable ADC interrupt in NVIC */
    NVIC_EnableIRQ(ADC_IRQn);
}

/**
 * @brief Start ADC conversion by software trigger.
 */
void ADC_StartConversion(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/**
 * @brief Retrieve the latest ADC conversion result.
 * @return uint16_t ADC conversion result (0-4095 for 12-bit)
 */
uint16_t ADC_GetValue(void)
{
    return adc_result;
}

/**
 * @brief ADC1 interrupt handler.
 *        Reads conversion result and clears EOC flag.
 */
void ADC_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_EOC)
    {
        adc_result = (uint16_t)(ADC1->DR & 0x0FFF);  // Read 12-bit result
        ADC1->SR &= ~ADC_SR_EOC;                     // Clear EOC flag (optional)
    }
}
