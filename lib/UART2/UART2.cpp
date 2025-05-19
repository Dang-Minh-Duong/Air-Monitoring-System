#include "UART2.h"
#include <stdint.h>
#include <stdbool.h>
#include "esp_attr.h"

#define IO_MUX_GPIO16_REG   *(volatile uint32_t *)0x3FF4904C
#define IO_MUX_GPIO17_REG   *(volatile uint32_t *)0x3FF49050

#define UART2_BASE          0x3FF6E000
#define UART_FIFO_REG       (*(volatile uint32_t *)(UART2_BASE + 0x00))
#define UART_INT_RAW_REG    (*(volatile uint32_t *)(UART2_BASE + 0x04))
#define UART_INT_ST_REG     (*(volatile uint32_t *)(UART2_BASE + 0x08))
#define UART_INT_ENA_REG    (*(volatile uint32_t *)(UART2_BASE + 0x0C))
#define UART_INT_CLR_REG    (*(volatile uint32_t *)(UART2_BASE + 0x10))
#define UART_CLKDIV_REG     (*(volatile uint32_t *)(UART2_BASE + 0x14))
#define UART_CONF0_REG      (*(volatile uint32_t *)(UART2_BASE + 0x20))
#define UART_CONF1_REG      (*(volatile uint32_t *)(UART2_BASE + 0x24))
#define UART_STATUS_REG     (*(volatile uint32_t *)(UART2_BASE + 0x1C))

#define UART_CLK_FREQ       80000000


/* ==== RX Buffer ==== */
#define UART_RX_BUFFER_SIZE     128
volatile uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint16_t uart_rx_head = 0;
volatile uint16_t uart_rx_tail = 0;

/**
 * @brief  Initialize USART2 for both TX and RX at baudRate.
 *         - TX pin: D17
 *         - RX pin: D16
 */

void UART2_Config(int baudRate)
{
    /* Config GPIO*/
    IO_MUX_GPIO16_REG |= (1 << 9);
    IO_MUX_GPIO17_REG |= (1 << 9);
    /*Config UART2*/
    /*
        - Baud rate = fclk / (16 * USARTDIV)
        - fclk = 80000000
    */
    // UART_CLKDIV_REG = UART_CLK_FREQ / (16 * baudRate);
    UART_CLKDIV_REG = 521;
    UART_CONF1_REG |= (1 << 0);
    UART_INT_ENA_REG = (1 << 0);  

}

void IRAM_ATTR UART2_ISR(void *arg)
{
    
    if (UART_INT_RAW_REG & (1 << 0))
    { 
        uint32_t rx_count = (UART_STATUS_REG) & 0xFF;

        /*Add data to FIFO*/ 
        while (rx_count--)
        {
            uint8_t data = (uint8_t)(UART_FIFO_REG & 0xFF);
            uint16_t next_head = (uart_rx_head + 1) % UART_RX_BUFFER_SIZE;

            if (next_head != uart_rx_tail)
            {
                uart_rx_buffer[uart_rx_head] = data;
                uart_rx_head = next_head;
            }
            else break;
        }

        /*Clear interrupt flag*/
        UART_INT_CLR_REG = (1 << 0); 
    }
}
/*Check read data available*/
bool UART2_Available(void) {
    return uart_rx_head != uart_rx_tail;
}
/*Read data*/
uint8_t UART2_GetChar(void) {
    while (!UART2_Available());

    uint8_t data = uart_rx_buffer[uart_rx_tail];
    uart_rx_tail = (uart_rx_tail + 1) % UART_RX_BUFFER_SIZE;
    return data;
}
