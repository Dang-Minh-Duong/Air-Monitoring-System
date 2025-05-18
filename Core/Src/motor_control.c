#include "MONITOR_CONTROL.h"
#include "stm32f4xx.h"

/**
 * @brief  Control motor direction and speed based on ADC input.
 *         - Adjust GPIO outputs (PA3, PA4, PA5) to select motor direction.
 *         - Adjust TIM2->CCR2 (PWM duty cycle) to control speed.
 *
 * @param  adc_value: 12-bit ADC value (0 to 4095)
 */
void Motor_Control(uint16_t adc_value)
{
    /* Clear PA3, PA4, PA5 (motor control pins) */
    GPIOA->BSRR = (1U << (3 + 16)) | (1U << (4 + 16)) | (1U << (5 + 16));

    if (adc_value < 1365) // < 1/3 Vref (~1.1V)
    {
        // Forward direction (PA3 = 1, PA4/PA5 = 0)
        GPIOA->BSRR |= (1U << 3);  // Set PA3
        TIM2->CCR2 = 250;          // Set PWM duty cycle (low speed)
    }
    else if (adc_value < 2730) // 1/3 to 2/3 Vref (~1.1V - ~2.2V)
    {
        // Neutral/stop (PA4 = 1, PA3/PA5 = 0)
        GPIOA->BSRR |= (1U << 4);  // Set PA4
        TIM2->CCR2 = 500;          // Medium speed (can be used for braking or idle)
    }
    else // > 2/3 Vref (~2.2V+)
    {
        // Reverse direction (PA5 = 1, PA3/PA4 = 0)
        GPIOA->BSRR |= (1U << 5);  // Set PA5
        TIM2->CCR2 = 750;          // High speed
    }
}
