#ifndef __ADC_INIT_H
#define __ADC_INIT_H

#include <stdint.h>

/**
 * @brief Initialize ADC1 on channel 0 (PA0) with specified resolution and sample time.
 *
 * @param resolution ADC resolution:
 *        - 0: 12-bit (default)
 *        - 1: 10-bit
 *        - 2: 8-bit
 *        - 3: 6-bit
 *
 * @param sample_time Sampling time (0 to 7):
 *        - 0: 3 cycles
 *        - 1: 15 cycles
 *        - 2: 28 cycles
 *        - 3: 56 cycles
 *        - 4: 84 cycles
 *        - 5: 112 cycles
 *        - 6: 144 cycles
 *        - 7: 480 cycles
 */
void ADC_init(uint8_t resolution, uint8_t sample_time);

/**
 * @brief Start ADC conversion using software trigger.
 */
void ADC_start_conversion(void);

/**
 * @brief Get the last ADC conversion result.
 *
 * @return uint16_t ADC value (0 to 4095 for 12-bit)
 */
uint16_t ADC_get_value(void);

/**
 * @brief ADC1 interrupt handler.
 *
 * This function must be called from the actual ADC IRQ handler defined in the vector table.
 */
void ADC_IRQHandler(void);

#endif /* __ADC_INIT_H */
