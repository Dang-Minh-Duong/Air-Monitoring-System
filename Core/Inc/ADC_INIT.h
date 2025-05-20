#ifndef ADC_INIT_H
#define ADC_INIT_H

#include <stdint.h>

void ADC_Init(uint8_t resolution, uint8_t sample_time);
void ADC_StartConversion(void);
uint16_t ADC_GetValue(void);

#endif
