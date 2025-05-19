#include <ADC_INIT.h>
#include <GPIO_INIT.h>
#include <MONITOR_CONTROL.h>
#include <TIM2_PWM.h>
#include "system_clock.h"

int main(void)
{
    SystemClock_Config();
    GPIO_Init();
    ADC_Init();
    TIM2_Init(1000, 999);

    while (1)
    {
        uint16_t adc_value = ADC_Read();
        Motor_Control(adc_value);
    }
}
