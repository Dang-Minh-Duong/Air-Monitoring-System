#ifndef TIM2_PWM_H
#define TIM2_PWM_H

#include <stdint.h>

/**
 * @file    TIM2_PWM.h
 * @brief   PWM initialization on TIM2 Channel 2 (PA1) for STM32F4 series.
 *
 * This module provides a function to configure TIM2 Channel 2 as PWM output.
 * It allows configurable prescaler (PSC) and auto-reload (ARR) values.
 * The PWM output is on pin PA1 (AF1).
 *
 * Formula:
 *   - PWM Frequency = Timer_Clock / ((PSC + 1) * (ARR + 1))
 *   - Duty Cycle (%) = (CCR2 / (ARR + 1)) * 100
 */

/**
 * @brief  Initialize TIM2 Channel 2 (PA1) as PWM output.
 *
 * @param  psc  Prescaler value (PSC)
 * @param  arr  Auto-reload value (ARR), determines PWM period
 *
 * Example:
 *   TIM2_Init(83, 999);  // With 84MHz clock → 1kHz PWM
 */
void TIM2_Init(uint16_t psc, uint16_t arr);

#endif /* TIM2_PWM_H */
