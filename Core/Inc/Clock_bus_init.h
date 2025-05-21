#ifndef __CLOCK_BUS_INIT_H
#define __CLOCK_BUS_INIT_H

/**
 * @brief Enable clocks for GPIOA, ADC1, and TIM2 peripherals.
 *
 * This function enables the required peripheral clocks using the RCC module.
 * - GPIOA: For analog input (e.g., PA0) and digital I/O (e.g., PA1 for PWM).
 * - ADC1 : For analog-to-digital conversion.
 * - TIM2 : For PWM signal generation or timing applications.
 */
void clock_bus_init(void);

#endif /* __CLOCK_BUS_INIT_H */
