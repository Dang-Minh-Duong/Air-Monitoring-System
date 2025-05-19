#include <ADC_INIT.h>
#include <GPIO_INIT.h>
#include <MONITOR_CONTROL.h>
#include <TIM2_PWM.h>
#include <USART1.h>
#include "system_clock.h"

int main(void)
{
    SystemClock_Config();
    GPIO_Init();
    ADC_Init();
    USART1_Config();
    TIM2_Init();

    while (1)
    {
        uint16_t adc_value = ADC_Read();
        USART1_SendString("ADC = ");
        USART1_SendNumber(adc_value);
        USART1_SendString("\r\n");
        Motor_Control(adc_value);
    }
}
