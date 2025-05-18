#ifndef USART1_H
#define USART1_H

#include <stdint.h>

void USART1_Config(void);
void USART1_SendChar(char c);
void USART1_SendString(char* str);
void USART1_SendNumber(uint16_t num);

#endif
