#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <stdint.h>

void adc_init(void);
int adc_read_gas(void);  // tương đương ADC_Read()

#endif
