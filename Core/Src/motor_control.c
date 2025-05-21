#include <motor_control.h>
#include "stm32f4xx.h"

/**
 * @brief  Control motor direction and speed based on ADC input and thresholds.
 *         - Adjust GPIO outputs (PA3, PA4, PA5) to select motor direction.
 *         - Adjust TIM2->CCR2 (PWM duty cycle) to control speed.
 *
 * @param  adc_value: 12-bit ADC value (0 to 4095)
 * @param  threshold1: lower threshold (e.g., 1/3 of 4095)
 * @param  threshold2: upper threshold (e.g., 2/3 of 4095)
 */
void motor_control(uint16_t adc_value, uint16_t threshold1, uint16_t threshold2)
{
    /* Clear PA3, PA4, PA5 (motor control pins) */
    GPIOA->BSRR = (1U << (3 + 16)) | (1U << (4 + 16)) | (1U << (5 + 16));

    if (adc_value < threshold1)
    {
        // Forward direction (PA3 = 1)
        GPIOA->BSRR |= (1U << 3);  // Set PA3
        TIM2->CCR2 = 250;          // Low speed
    }
    else if (adc_value < threshold2)
    {
        // Neutral/stop (PA4 = 1)
        GPIOA->BSRR |= (1U << 4);  // Set PA4
        TIM2->CCR2 = 500;          // Medium speed
    }
    else
    {
        // Reverse direction (PA5 = 1)
        GPIOA->BSRR |= (1U << 5);  // Set PA5
        TIM2->CCR2 = 750;          // High speed
    }
}
