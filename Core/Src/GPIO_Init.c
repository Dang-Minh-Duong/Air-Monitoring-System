#include "GPIO_INIT.h"
#include "stm32f4xx.h"

/**
 * @brief  Initialize GPIOA:
 *         - PA0 as analog input (for ADC)
 *         - PA1 as alternate function AF1 (for TIM2 CH2)
 *         - PA2 as general purpose output (push-pull, low)
 *         - PA3, PA4, PA5 as general purpose outputs (for LEDs)
 */
void GPIO_Init(void) {
	GPIO_ADC_init();
	GPIO_PWM_init();
	GPIO_LED_init();
}



void GPIO_ADC_init(void) {
	GPIOA->MODER |= (3U << (0 * 2));         // MODER0[1:0] = 11: Analog mode
	GPIOA->PUPDR &= ~(3U << (0 * 2));        // No pull-up, no pull-down
}

void GPIO_PWM_init(void) {
	GPIOA->MODER &= ~(3U << (1 * 2)); // Clear MODER1
	GPIOA->MODER |=  (2U << (1 * 2)); // Alternate function
	GPIOA->AFR[0] &= ~(0xF << (1 * 4)); // Clear AFRL1
	GPIOA->AFR[0] |=  (0x1 << (1 * 4)); // AF1 (TIM2)

	/* 4. Configure PA2 as general purpose output */
	GPIOA->MODER &= ~(3U << (2 * 2)); // Clear MODER2
	GPIOA->MODER |=  (1U << (2 * 2)); // General output
	GPIOA->OTYPER &= ~(1U << 2);      // Push-pull
	GPIOA->OSPEEDR |= (3U << (2 * 2)); // High speed
	GPIOA->PUPDR &= ~(3U << (2 * 2)); // No pull-up/pull-down
	GPIOA->BSRR = (1U << (2 + 16));   // Reset PA2 (set to low)
}

void GPIO_LED_init(void) {
	for (int pin = 3; pin <= 5; pin++) {
		GPIOA->MODER &= ~(3U << (pin * 2)); // Clear MODERx
		GPIOA->MODER |=  (1U << (pin * 2)); // Set as output
		GPIOA->OTYPER &= ~(1U << pin);      // Push-pull
		GPIOA->OSPEEDR |= (3U << (pin * 2)); // High speed
		GPIOA->PUPDR &= ~(3U << (pin * 2)); // No pull-up/pull-down
		GPIOA->BSRR = (1U << (pin + 16));   // Reset pin (set to low)
	    }
}
