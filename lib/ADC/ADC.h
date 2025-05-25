#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// ADC unit selection
typedef enum { ADC_UNIT_1 = 1, ADC_UNIT_2 = 2 } adc_unit_t;

// ADC channel (0-7 for ADC1, 0-9 for ADC2)
typedef uint8_t adc_channel_t;

// ADC resolution (bit width)
typedef enum {
    ADC_WIDTH_9Bit = 0,
    ADC_WIDTH_10Bit = 1,
    ADC_WIDTH_11Bit = 2,
    ADC_WIDTH_12Bit = 3
} adc_resolution_t;

// ADC input attenuation
typedef enum {
    ADC_ATTEN_0DB = 0,   // 00: 0 dB
    ADC_ATTEN_3DB = 1,   // 01: 3 dB
    ADC_ATTEN_6DB = 2,   // 10: 6 dB
    ADC_ATTEN_11DB = 3   // 11: 11 dB 
} adc_atten_t;

// Base address for RTC (SENS) registers (RTC SAR ADC controller)
#define SENS_BASE           0x3FF48800

// SAR ADC1 control registers (see TRM v5.3):contentReference[oaicite:13]{index=13}
#define SENS_SAR_READ_CTRL_REG   (*(volatile uint32_t *)(SENS_BASE + 0x00))
#define SENS_SAR_MEAS_START1_REG (*(volatile uint32_t *)(SENS_BASE + 0x54))
#define SENS_SAR_ATTEN1_REG      (*(volatile uint32_t *)(SENS_BASE + 0x34))

// SAR ADC2 control registers:contentReference[oaicite:14]{index=14}
#define SENS_SAR_READ_CTRL2_REG   (*(volatile uint32_t *)(SENS_BASE + 0x90))
#define SENS_SAR_MEAS_START2_REG  (*(volatile uint32_t *)(SENS_BASE + 0x94))
#define SENS_SAR_ATTEN2_REG       (*(volatile uint32_t *)(SENS_BASE + 0x38))

// Common SAR ADC control register (bit width and stop bits):contentReference[oaicite:15]{index=15}
#define SENS_SAR_START_FORCE_REG  (*(volatile uint32_t *)(SENS_BASE + 0x2C))

// Bit positions in SENS_SAR_START_FORCE_REG
#define SENS_SAR1_BIT_WIDTH_POS   0   // ADC1 bit-width field (2 bits)
#define SENS_SAR2_BIT_WIDTH_POS   2   // ADC2 bit-width field (2 bits)
#define SENS_SAR1_BIT_WIDTH_MASK  (0x3u << SENS_SAR1_BIT_WIDTH_POS)
#define SENS_SAR2_BIT_WIDTH_MASK  (0x3u << SENS_SAR2_BIT_WIDTH_POS)

// Bit positions in SENS_SAR_READ_CTRL_REG (ADC1):contentReference[oaicite:16]{index=16}
#define SENS_SAR1_DATA_INV      28  // invert data (not used)
#define SENS_SAR1_DIG_FORCE     27  // 1 = use DIG ADC1 CTRL, 0 = RTC control
#define SENS_SAR1_SAMPLE_BIT    16  // ADC1 sample bit-width (3 bits):contentReference[oaicite:17]{index=17}
#define SENS_SAR1_SAMPLE_CYCLE  8  // ADC1 sample cycles (9 bits):contentReference[oaicite:18]{index=18}
#define SENS_SAR1_CLK_DIV        0  // ADC1 clock divider (7 bits)

// Masks for ADC1 sample bit/cycle fields
#define SENS_SAR1_SAMPLE_BIT_MASK   (0x3u << SENS_SAR1_SAMPLE_BIT)
#define SENS_SAR1_SAMPLE_CYCLE_MASK (0xFFu << SENS_SAR1_SAMPLE_CYCLE)

// Bit positions in SENS_SAR_READ_CTRL2_REG (ADC2)
#define SENS_SAR2_DATA_INV      29
#define SENS_SAR2_DIG_FORCE     28
#define SENS_SAR2_SAMPLE_BIT    16  // ADC2 sample bit-width (3 bits)
#define SENS_SAR2_SAMPLE_CYCLE   8  // ADC2 sample cycles (9 bits)
#define SENS_SAR2_CLK_DIV        0  // ADC2 clock divider (7 bits)

// Masks for ADC2 sample bit/cycle fields
#define SENS_SAR2_SAMPLE_BIT_MASK   (0x3u << SENS_SAR2_SAMPLE_BIT)
#define SENS_SAR2_SAMPLE_CYCLE_MASK (0xFFu << SENS_SAR2_SAMPLE_CYCLE)

// Bit positions in SENS_SAR_MEAS_START1_REG (ADC1 start/control)
#define SENS_SAR1_EN_PAD_FORCE  31  // 1 = software controls ADC1 pad enable
#define SENS_SAR1_EN_PAD        19  // ADC1 pad enable bitmap (channel enable bits)
#define SENS_MEAS1_START_FORCE  18  // 1 = software starts ADC1 conversion
#define SENS_MEAS1_START_SAR    17  // trigger ADC1 start (edge-triggered)
#define SENS_MEAS1_DONE_SAR     16  // ADC1 done flag (RO)
#define SENS_MEAS1_DATA_SAR      0  // ADC1 data (12 bits) (RO)

// Bit positions in SENS_SAR_MEAS_START2_REG (ADC2 start/control)
#define SENS_SAR2_EN_PAD_FORCE  31  // 1 = software controls ADC2 pad enable
#define SENS_SAR2_EN_PAD        19  // ADC2 pad enable bitmap
#define SENS_MEAS2_START_FORCE  18  // 1 = software starts ADC2 conversion
#define SENS_MEAS2_START_SAR    17  // trigger ADC2 start
#define SENS_MEAS2_DONE_SAR     16  // ADC2 done flag (RO)
#define SENS_MEAS2_DATA_SAR      0  // ADC2 data (12 bits) (RO)

// Function prototypes
void adc_configure(adc_unit_t adc, adc_channel_t ch, adc_resolution_t res,
                   adc_atten_t atten, uint8_t sample_cycle);
uint16_t adc_read(adc_unit_t adc);

#endif // ADC_H
