#include "gpio_address.h"
#include "led.h"
#include <stdint.h>

/*
 * @brief Control three LEDs based on a value and two threshold levels.
 *
 * This function turns on one LED and turns off the other two depending
 * on whether the value is below t1, between t1 and t2, or above t2.
 *
 * @param led1 GPIO number of the first LED.
 * @param led2 GPIO number of the second LED.
 * @param led3 GPIO number of the third LED.
 * @param value The value to compare with thresholds.
 * @param t1 Threshold 1.
 * @param t2 Threshold 2.
 */
void ledControl(int led1, int led2, int led3, uint16_t value, uint16_t t1, uint16_t t2)
{
    /* good air quality*/
    if (value < t1)
    {
        /* Turn ON led1, turn OFF led2 and led3 */
        GPIO_OUT_W1TS_REG = (1 << led1);  
        GPIO_OUT_W1TC_REG = (1 << led2);  
        GPIO_OUT_W1TC_REG = (1 << led3);  
    }
    /* normal air quality*/
    else if (value < t2)
    {
        /* Turn ON led2, turn OFF led1 and led3 */
        GPIO_OUT_W1TS_REG = (1 << led2);  
        GPIO_OUT_W1TC_REG = (1 << led1);  
        GPIO_OUT_W1TC_REG = (1 << led3);  
    }
    /* bad air quality*/
    else
    {
        /* Turn ON led3, turn OFF led1 and led2 */
        GPIO_OUT_W1TS_REG = (1 << led3);  
        GPIO_OUT_W1TC_REG = (1 << led1);  
        GPIO_OUT_W1TC_REG = (1 << led2); 
    }
}
