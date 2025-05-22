#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_init();
uint16_t adc_read(int channel);

#endif
