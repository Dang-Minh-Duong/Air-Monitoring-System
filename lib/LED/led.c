
#include "gpio_address.h"
#include <stdint.h>
void ledControl(int led1, int led2, int led3, uint16_t value, uint16_t t1, uint16_t t2)
{
    if (value < t1)
    {

        GPIO_OUT_W1TS_REG = (1 << led1);  
        GPIO_OUT_W1TC_REG = (1 << led2);  
        GPIO_OUT_W1TC_REG = (1 << led3);  
    }
    else if (value < t2)
    {

        GPIO_OUT_W1TS_REG = (1 << led2);  
        GPIO_OUT_W1TC_REG = (1 << led1);  
        GPIO_OUT_W1TC_REG = (1 << led3);  
    }
    else
    {
     
        GPIO_OUT_W1TS_REG = (1 << led3);  
        GPIO_OUT_W1TC_REG = (1 << led1);  
        GPIO_OUT_W1TC_REG = (1 << led2); 
    }
}
