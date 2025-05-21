#include "GPIO_INIT.h"
#include "stm32f4xx.h"

/**
 * @brief  Initialize GPIOA pins used in the application.
 *         - PA0 : Analog input (ADC)
 *         - PA1 : Alternate function TIM2_CH2 (PWM output)
 *         - PA2 : General purpose output (digital output) used for control motor direction
 *         - PA3, PA4, PA5 : General purpose outputs (LED control)
 */
void GPIO_init(void) {
    GPIO_ADC_init();
    GPIO_PWM_init(1);
    GPIO_LED_init();
}

/**
 * @brief  Configure PA0 as analog input for ADC channel 0.
 *         - Mode: Analog
 *         - No pull-up, no pull-down
 */
void GPIO_ADC_init(void) {
    // Set PA0 to analog mode
    GPIOA->MODER |= (3U << (0 * 2));          // MODER0 = 11 (Analog)
    GPIOA->PUPDR &= ~(3U << (0 * 2));         // PUPDR0 = 00 (No pull-up/down)
}

/**
 * @brief  Configure:
 *         - PA1 as alternate function AF1 (TIM2_CH2 for PWM signal)Q
 *         - PA2 as general purpose output to control motor direction.
 * @param  direction: Initial direction state of motor control pin (PA2).
 *         - 0: Output LOW (e.g., clockwise)
 *         - 1: Output HIGH (e.g., counterclockwise)
 */
void GPIO_PWM_init(uint8_t direction) {
    // PA1: Alternate function mode (AF1 = TIM2_CH2)
    GPIOA->MODER &= ~(3U << (1 * 2));         // Clear MODER1
    GPIOA->MODER |=  (2U << (1 * 2));         // Set MODER1 = 10 (AF mode)
    GPIOA->AFR[0] &= ~(0xF << (1 * 4));       // Clear AFRL1
    GPIOA->AFR[0] |=  (0x1 << (1 * 4));       // Set AF1 (TIM2_CH2)

    // PA2: General-purpose output (motor direction)
    GPIOA->MODER &= ~(3U << (2 * 2));         // Clear MODER2
    GPIOA->MODER |=  (1U << (2 * 2));         // Set MODER2 = 01 (output)
    GPIOA->OTYPER &= ~(1U << 2);              // Push-pull
    GPIOA->OSPEEDR |= (3U << (2 * 2));        // High speed
    GPIOA->PUPDR &= ~(3U << (2 * 2));         // No pull-up/down

    if (direction)
        GPIOA->BSRR = (1U << 2);              // Set PA2 (HIGH)
    else
        GPIOA->BSRR = (1U << (2 + 16));       // Reset PA2 (LOW)
}


/**
 * @brief  Configure PA3, PA4, PA5 as digital outputs for controlling LEDs.
 *         - Output type: Push-pull
 *         - Speed: High
 *         - Initial state: LOW
 */
void GPIO_LED_init(void) {
    for (int pin = 3; pin <= 5; pin++) {
        GPIOA->MODER &= ~(3U << (pin * 2));       // Clear MODERx
        GPIOA->MODER |=  (1U << (pin * 2));       // Set as output (01)
        GPIOA->OTYPER &= ~(1U << pin);            // Push-pull
        GPIOA->OSPEEDR |= (3U << (pin * 2));      // High speed
        GPIOA->PUPDR &= ~(3U << (pin * 2));       // No pull-up/down
        GPIOA->BSRR = (1U << (pin + 16));         // Reset output (set LOW)
    }
}
