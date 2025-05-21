#include "TIM2_PWM.h"
#include "stm32f4xx.h"

/**
 * @brief  Initialize TIM2 PWM on Channel 2 (PA1) with configurable PSC and ARR.
 * @param  psc: Prescaler value (PSC)
 * @param  arr: Auto-reload value (ARR) → determines PWM period
 *         PWM frequency = Timer_Clock / ((PSC + 1) * (ARR + 1))
 *         Duty cycle = CCR2 / (ARR + 1)
 */
void TIM2_init(uint16_t psc, uint16_t arr)
{
    /* 2. Reset TIM2 configuration */
    TIM2->CR1 = 0;
    TIM2->CCMR1 = 0;
    TIM2->CCER = 0;
    TIM2->CNT = 0;

    /* 3. Set prescaler and auto-reload */
    TIM2->PSC = psc;
    TIM2->ARR = arr;
    TIM2->CCR2 = 0;  // Initial duty cycle = 0%

    /* 4. Set PWM Mode 1 on Channel 2 (OC2M = 110), enable preload */
    TIM2->CCMR1 |= (6 << 12);       // OC2M = 110: PWM mode 1
    TIM2->CCMR1 |= TIM_CCMR1_OC2PE; // Enable preload for CCR2

    /* 5. Enable channel 2 output */
    TIM2->CCER |= TIM_CCER_CC2E;

    /* 6. Generate update event to load ARR/PSC into shadow registers */
    TIM2->EGR |= TIM_EGR_UG;

    /* 7. Enable TIM2 counter */
    TIM2->CR1 |= TIM_CR1_CEN;
}
