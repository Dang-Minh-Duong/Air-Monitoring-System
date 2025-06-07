#include "ADC.h"

static adc_channel_t adc1_channel;
static adc_channel_t adc2_channel;



void adc_configure(adc_unit_t adc, adc_channel_t ch, adc_resolution_t res,
                   adc_atten_t atten, uint8_t sample_cycle)
{
    /*Set ADC1 or ADC2 channel selection for use in adc_read()*/ 
    if (adc == ADC_UNIT_1) {
        adc1_channel = ch;
        /*Configure bit width: set SAR1 and ADC1 sample-bit fields (0-3 for 9-12 bits)*/
        uint32_t sf = SENS_SAR_START_FORCE_REG;
        sf &= ~SENS_SAR1_BIT_WIDTH_MASK;
        sf |= ((uint32_t)res << SENS_SAR1_BIT_WIDTH_POS);
        SENS_SAR_START_FORCE_REG = sf;
        /* In READ_CTRL1, set sample bit-width and sample cycle*/
        uint32_t rc = SENS_SAR_READ_CTRL_REG;
        rc &= ~(SENS_SAR1_SAMPLE_BIT_MASK | SENS_SAR1_SAMPLE_CYCLE_MASK);
        rc |= ((uint32_t)res << SENS_SAR1_SAMPLE_BIT);
        rc |= ((uint32_t)(sample_cycle & 0xFF) << SENS_SAR1_SAMPLE_CYCLE);
        /*Ensure ADC1 is controlled by RTC (clear DIG_FORCE)*/
        rc &= ~(1u << SENS_SAR1_DIG_FORCE);
        /*Set not inv data*/
        rc |= 1u << SENS_SAR1_DATA_INV;  
        SENS_SAR_READ_CTRL_REG = rc;
        /*Set attenuation for this channel in SENS_SAR_ATTEN1_REG*/
        uint32_t att = SENS_SAR_ATTEN1_REG;
        att &= ~(0x3u << (ch * 2)); 
        att |= ((uint32_t)atten << (ch * 2));
        SENS_SAR_ATTEN1_REG = att;
    } 
    else if (adc == ADC_UNIT_2) {
        adc2_channel = ch;
        /*Configure bit width in START_FORCE_REG for ADC2*/
        uint32_t sf = SENS_SAR_START_FORCE_REG;
        sf &= ~SENS_SAR2_BIT_WIDTH_MASK;
        sf |= ((uint32_t)res << SENS_SAR2_BIT_WIDTH_POS);
        SENS_SAR_START_FORCE_REG = sf;
        /*Configure sample-bit and sample-cycle in READ_CTRL2*/
        uint32_t rc = SENS_SAR_READ_CTRL2_REG;
        rc &= ~(SENS_SAR2_SAMPLE_BIT_MASK | SENS_SAR2_SAMPLE_CYCLE_MASK);
        rc |= ((uint32_t)res << SENS_SAR2_SAMPLE_BIT);
        rc |= ((uint32_t)(sample_cycle & 0xFF) << SENS_SAR2_SAMPLE_CYCLE);
        /*Ensure ADC2 is controlled by RTC (clear DIG_FORCE)*/
        rc &= ~(1u << SENS_SAR2_DIG_FORCE);
        /*Set not inv data*/
        rc |= 1u << SENS_SAR2_DATA_INV;  
        SENS_SAR_READ_CTRL2_REG = rc;
        /*Set attenuation for this channel in SENS_SAR_ATTEN2_REG*/
        uint32_t att = SENS_SAR_ATTEN2_REG;
        att &= ~(0x3u << (ch * 2));
        att |= ((uint32_t)atten << (ch * 2));
        SENS_SAR_ATTEN2_REG = att;
    }
}

uint16_t adc_read(adc_unit_t adc)
{
    if (adc == ADC_UNIT_1) {
        /*Configure pad control: enable SW control and set channel bit*/
        SENS_SAR_MEAS_START1_REG |= (1u << SENS_SAR1_EN_PAD_FORCE) | (1u << SENS_MEAS1_START_FORCE);
        /*Select channel by setting corresponding bit in EN_PAD*/ 
        SENS_SAR_MEAS_START1_REG &= ~(((uint32_t)0xFFF) << SENS_SAR1_EN_PAD);
        SENS_SAR_MEAS_START1_REG |= (1u << (SENS_SAR1_EN_PAD + adc1_channel));
        /*Start conversion: toggle START_SAR bit*/
        SENS_SAR_MEAS_START1_REG &= ~(1u << SENS_MEAS1_START_SAR);
        SENS_SAR_MEAS_START1_REG |=  (1u << SENS_MEAS1_START_SAR);
        /*Wait for conversion to complete (DONE flag)*/
        while ((SENS_SAR_MEAS_START1_REG & (1u << SENS_MEAS1_DONE_SAR)) == 0) { }
        /*Read 12-bit result from bits [11:0]*/
        return (uint16_t)(SENS_SAR_MEAS_START1_REG & 0x0FFFu);
    } 
    else { 
        /*Configure as same as ADC1*/
        SENS_SAR_MEAS_START2_REG |= (1u << SENS_SAR2_EN_PAD_FORCE) | (1u << SENS_MEAS2_START_FORCE);
        SENS_SAR_MEAS_START2_REG &= ~(((uint32_t)0xFFF) << SENS_SAR2_EN_PAD);
        SENS_SAR_MEAS_START2_REG |= (1u << (SENS_SAR2_EN_PAD + adc2_channel));
        SENS_SAR_MEAS_START2_REG &= ~(1u << SENS_MEAS2_START_SAR);
        SENS_SAR_MEAS_START2_REG |=  (1u << SENS_MEAS2_START_SAR);
        while ((SENS_SAR_MEAS_START2_REG & (1u << SENS_MEAS2_DONE_SAR)) == 0) { }
        return (uint16_t)(SENS_SAR_MEAS_START2_REG & 0x0FFFu);
    }
}
