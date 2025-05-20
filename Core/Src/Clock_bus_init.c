#include "Clock_bus_init.h"
#include "stm32f4xx.h"

void Clock_bus_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;     // Enable GPIOA clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;      // Enable ADC1 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}
