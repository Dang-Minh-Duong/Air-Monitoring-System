/*
 * pinDriver.c
 *
 *  Created on: May 19, 2025
 *      Author: Administrator
 */
#include "pinDriver.h"

void Pin_Init_All(void) {
    // Enable toàn bộ GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    Pin_ADC_Init();
    Pin_PWM_Init();
    Pin_Motor_Init();
    Pin_LED_Init();
    Pin_USART1_Init();
}

void Pin_ADC_Init(void) {
    /*PA0: Analog mode*/
    GPIOA->MODER |= (3 << 0); /*11: Analog mode*/
}

void Pin_PWM_Init(void) {
    /* PA1: AF mode, TIM2_CH2*/
    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->MODER |= (2 << (1 * 2)); /* Alternate function*/

    GPIOA->AFR[0] &= ~(0xF << 4);  /*Clear*/
    GPIOA->AFR[0] |= (1 << 4);     /* AF1 for TIM2*/
}

void Pin_Motor_Init(void) {
    /*PA2: Output cho IN2*/
    GPIOA->MODER &= ~(3 << (2 * 2));
    GPIOA->MODER |= (1 << (2 * 2)); /*Output mode*/

    GPIOA->BSRR = GPIO_BSRR_BR2; /*Đặt thấp*/
}

void Pin_LED_Init(void) {
    /* PA3, PA4, PA5: Output mode*/
    GPIOA->MODER |= (1 << 6) | (1 << 8) | (1 << 10); /*PA3, PA4, PA5*/
}

void Pin_USART1_Init(void) {
    /* PA9 (TX) – USART1 TX*/
    GPIOA->MODER &= ~(3 << (9 * 2));
    GPIOA->MODER |= (2 << (9 * 2)); /*Alternate function*/

    GPIOA->AFR[1] &= ~(0xF << 4);  /*AFRH[1] = PA9*/
    GPIOA->AFR[1] |= (7 << 4);     /*AF7 = USART1*/
}


