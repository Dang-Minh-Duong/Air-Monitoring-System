#include "ADC.h"

#define SENS_BASE                   0x3FF48800
#define SENS_SAR_READ_CTRL_REG     (*(volatile uint32_t *)(SENS_BASE + 0x00))
#define SENS_SAR_MEAS_START1_REG   (*(volatile uint32_t *)(SENS_BASE + 0x54))
#define SENS_SAR_ATTEN1_REG        (*(volatile uint32_t *)(SENS_BASE + 0x34))


#define SENS_SAR1_EN_PAD_FORCE     (1 << 31)  
#define SENS_MEAS1_START_FORCE     (1 << 18)  
#define SENS_MEAS1_START_SAR       (1 << 17)  
#define SENS_MEAS1_DONE_SAR        (1 << 16)  
#define SENS_MEAS1_DATA_MASK       0x3FFF     




void adc_init() {
    
    SENS_SAR_READ_CTRL_REG |= (3 << 16); // sample_bit = 11 (12-bit)
    SENS_SAR_READ_CTRL_REG |= (2 << 0);  // clk_div = 2
}

uint16_t adc_read(int channel, uint8_t atten) {
    if (channel < 0 || channel > 7) return 0; // Validate channel (0-7 for ADC1)

    
    uint32_t atten_shift = channel * 2; 
    uint32_t atten_mask = ~(0x3 << atten_shift); 
    SENS_SAR_ATTEN1_REG = (SENS_SAR_ATTEN1_REG & atten_mask) | ((atten & 0x3) << atten_shift);

 
    SENS_SAR_MEAS_START1_REG |= SENS_SAR1_EN_PAD_FORCE;


    SENS_SAR_MEAS_START1_REG |= SENS_MEAS1_START_FORCE;
    SENS_SAR_MEAS_START1_REG |= SENS_MEAS1_START_SAR;


    while (!(SENS_SAR_MEAS_START1_REG & SENS_MEAS1_DONE_SAR)) {}


    uint16_t adc_data = SENS_SAR_MEAS_START1_REG & SENS_MEAS1_DATA_MASK;
    return adc_data & 0xFFF; // Mask to 12-bit (discard upper 2 bits if any)
}