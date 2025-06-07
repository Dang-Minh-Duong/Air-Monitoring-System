#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* ADC unit selection */
typedef enum {
    ADC_UNIT_1 = 1,
    ADC_UNIT_2 = 2
} adc_unit_t;

/* ADC channel type (0-7 for ADC1, 0-9 for ADC2) */
typedef uint8_t adc_channel_t;

/* ADC resolution options */
typedef enum {
    ADC_WIDTH_9Bit  = 0,
    ADC_WIDTH_10Bit = 1,
    ADC_WIDTH_11Bit = 2,
    ADC_WIDTH_12Bit = 3
} adc_resolution_t;

/* ADC input attenuation */
typedef enum {
    ADC_ATTEN_0DB  = 0,
    ADC_ATTEN_3DB  = 1,
    ADC_ATTEN_6DB  = 2,
    ADC_ATTEN_11DB = 3
} adc_atten_t;

/* Base address for RTC SAR ADC registers */
#define SENS_BASE 0x3FF48800

/* SAR ADC1 control registers */
#define SENS_SAR_READ_CTRL_REG     (*(volatile uint32_t *)(SENS_BASE + 0x00))
#define SENS_SAR_MEAS_START1_REG   (*(volatile uint32_t *)(SENS_BASE + 0x54))
#define SENS_SAR_ATTEN1_REG        (*(volatile uint32_t *)(SENS_BASE + 0x34))

/* SAR ADC2 control registers */
#define SENS_SAR_READ_CTRL2_REG    (*(volatile uint32_t *)(SENS_BASE + 0x90))
#define SENS_SAR_MEAS_START2_REG   (*(volatile uint32_t *)(SENS_BASE + 0x94))
#define SENS_SAR_ATTEN2_REG        (*(volatile uint32_t *)(SENS_BASE + 0x38))

/* Common SAR ADC control register (bit width control) */
#define SENS_SAR_START_FORCE_REG   (*(volatile uint32_t *)(SENS_BASE + 0x2C))

/* Bit positions and masks in SENS_SAR_START_FORCE_REG */
#define SENS_SAR1_BIT_WIDTH_POS    0
#define SENS_SAR2_BIT_WIDTH_POS    2
#define SENS_SAR1_BIT_WIDTH_MASK   (0x3u << SENS_SAR1_BIT_WIDTH_POS)
#define SENS_SAR2_BIT_WIDTH_MASK   (0x3u << SENS_SAR2_BIT_WIDTH_POS)

/* Bit positions in SENS_SAR_READ_CTRL_REG (ADC1) */
#define SENS_SAR1_DATA_INV         28
#define SENS_SAR1_DIG_FORCE        27
#define SENS_SAR1_SAMPLE_BIT       16
#define SENS_SAR1_SAMPLE_CYCLE     8

/* Masks for ADC1 sample bit and sample cycle fields */
#define SENS_SAR1_SAMPLE_BIT_MASK  (0x3u << SENS_SAR1_SAMPLE_BIT)
#define SENS_SAR1_SAMPLE_CYCLE_MASK (0xFFu << SENS_SAR1_SAMPLE_CYCLE)

/* Bit positions in SENS_SAR_READ_CTRL2_REG (ADC2) */
#define SENS_SAR2_DATA_INV         29
#define SENS_SAR2_DIG_FORCE        28
#define SENS_SAR2_SAMPLE_BIT       16
#define SENS_SAR2_SAMPLE_CYCLE     8

/* Masks for ADC2 sample bit and sample cycle fields */
#define SENS_SAR2_SAMPLE_BIT_MASK  (0x3u << SENS_SAR2_SAMPLE_BIT)
#define SENS_SAR2_SAMPLE_CYCLE_MASK (0xFFu << SENS_SAR2_SAMPLE_CYCLE)

/* Bit positions in SENS_SAR_MEAS_START1_REG (ADC1 start/control) */
#define SENS_SAR1_EN_PAD_FORCE     31
#define SENS_SAR1_EN_PAD           19
#define SENS_MEAS1_START_FORCE     18
#define SENS_MEAS1_START_SAR       17
#define SENS_MEAS1_DONE_SAR        16

/* Bit positions in SENS_SAR_MEAS_START2_REG (ADC2 start/control) */
#define SENS_SAR2_EN_PAD_FORCE     31
#define SENS_SAR2_EN_PAD           19
#define SENS_MEAS2_START_FORCE     18
#define SENS_MEAS2_START_SAR       17
#define SENS_MEAS2_DONE_SAR        16

/* Function prototypes */

/**
 * @brief Configure the ADC unit with channel, resolution, attenuation and sample cycle.
 *
 * @param adc          ADC unit (ADC_UNIT_1 or ADC_UNIT_2).
 * @param ch           ADC channel number (0-7 for ADC1, 0-9 for ADC2).
 * @param res          ADC resolution (ADC_WIDTH_9Bit to ADC_WIDTH_12Bit).
 * @param atten        Input attenuation (ADC_ATTEN_0DB to ADC_ATTEN_11DB).
 * @param sample_cycle Sample time in ADC clock cycles (usually 1-255).
 */
void adc_configure(adc_unit_t adc, adc_channel_t ch, adc_resolution_t res,
                   adc_atten_t atten, uint8_t sample_cycle);

/**
 * @brief Start ADC conversion and return 12-bit conversion result.
 *
 * @param adc ADC unit to read from (ADC_UNIT_1 or ADC_UNIT_2).
 * @return 12-bit ADC conversion result (0-4095).
 */
uint16_t adc_read(adc_unit_t adc);

#endif /* ADC_H */
