#include <ADC_init.h>
#include <GPIO_init.h>
#include <motor_control.h>
#include <TIM2_PWM.h>
#include "system_clock.h"
#include "clock_bus_init.h"

int main(void)
{
    SystemClock_Config();
    clock_bus_init();

    GPIO_init();
    ADC_init(1, 3);
    TIM2_init(1000, 999);

    while (1)
    {
    	ADC_start_conversion();
        uint16_t adc_value = ADC_get_value();
        motor_control(adc_value, 1365, 2730);
    }
}
