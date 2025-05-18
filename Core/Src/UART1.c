#include "USART1.h"
#include "stm32f4xx.h"
#include <stdio.h>

/**
 * @brief  Initialize USART1 for TX only at 9600 baud.
 *         - TX pin: PA9 (AF7)
 *         - Baudrate: 9600 (assuming 16 MHz APB2 clock)
 */
void USART1_Config(void)
{
    /* 1. Enable GPIOA and USART1 clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;      // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;     // Enable USART1 clock

    /* 2. Configure PA9 as Alternate Function (AF7 for USART1_TX) */
    GPIOA->MODER &= ~(0x3 << (9 * 2));        // Clear mode bits
    GPIOA->MODER |=  (0x2 << (9 * 2));        // Set to Alternate Function
    GPIOA->AFR[1] &= ~(0xF << ((9 - 8) * 4));  // Clear AFRH9
    GPIOA->AFR[1] |=  (0x7 << ((9 - 8) * 4));  // Set AF7 (USART1) for PA9

    /* 3. Configure USART1 parameters */
    // Baud rate = fclk / (16 * USARTDIV)
    // Assuming fclk = 16 MHz, USARTDIV = 104.1875 => BRR = 0x683
    USART1->BRR = 0x0683;                     // Set baudrate to 9600

    USART1->CR1 = 0;                          // Reset CR1
    USART1->CR1 |= USART_CR1_TE;             // Enable Transmitter
    USART1->CR1 |= USART_CR1_UE;             // Enable USART
}

/**
 * @brief  Send a single character over USART1.
 * @param  c: character to send
 */
void USART1_SendChar(char c)
{
    while (!(USART1->SR & USART_SR_TXE));     // Wait until transmit buffer empty
    USART1->DR = c;                           // Send character
}

/**
 * @brief  Send a null-terminated string over USART1.
 * @param  str: pointer to string
 */
void USART1_SendString(char* str)
{
    while (*str)
    {
        USART1_SendChar(*str++);
    }
}

/**
 * @brief  Send an unsigned integer number as ASCII over USART1.
 * @param  num: number to send
 */
void USART1_SendNumber(uint16_t num)
{
    char buffer[10];
    sprintf(buffer, "%u", num);
    USART1_SendString(buffer);
}
