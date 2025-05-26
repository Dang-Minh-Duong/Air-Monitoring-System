#ifndef PIN_H
#define PIN_H

#include <stdint.h>
#include <ADC.h>
/**
 * Configure RTC_IO pin for ADC analog input (via RTC SAR ADC).
 * This enables input, disables pull-up/down, disables hold, and sets function to RTC.
 *
 * @param adc      ADC unit (ADC_UNIT_1 or ADC_UNIT_2)
 * @param rtc_pin  RTC_IO pin number (0–17)
 */
void adc_configure_pin(adc_unit_t adc, int rtc_pin);

#endif /* PIN_H */
