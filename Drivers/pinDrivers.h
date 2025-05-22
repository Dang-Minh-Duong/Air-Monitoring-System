/*
 * pinDriver.h
 *
 *  Created on: May 19, 2025
 *      Author: Administrator
 */

/*#ifndef PINDRIVER_H_
/*#define PINDRIVER_H_



/*#endif /* PINDRIVER_H_ */
#ifndef __PIN_DRIVER_H
#define __PIN_DRIVER_H

#include "stm32f4xx.h"

/*Khởi tạo tất cả GPIO cần dùng*/
void Pin_Init_All(void);

/*Cấu hình GPIO cho motor*/
void Pin_Motor_Init(void);

/* Cấu hình GPIO cho LED*/
void Pin_LED_Init(void);

/* Cấu hình GPIO cho UART (USART1)*/
void Pin_USART1_Init(void);

/* Cấu hình GPIO cho ADC (PA0)*/
void Pin_ADC_Init(void);

/* Cấu hình PWM chân TIM2_CH2 (PA1)*/
void Pin_PWM_Init(void);

#endif
