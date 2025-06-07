#include "ADC.h"

/* Store ADC channel selection for ADC1 and ADC2 */
static adc_channel_t adc1_channel;
static adc_channel_t adc2_channel;

/**
 * @brief Configure ADC unit and channel with specified resolution, attenuation, and sample cycle.
 * 
 * @param adc          ADC unit (ADC_UNIT_1 or ADC_UNIT_2)
 * @param ch           ADC channel to select
 * @param res          ADC resolution (bit width)
 * @param atten        ADC input attenuation
 * @param sample_cycle ADC sampling cycles
 */
void adc_configure(adc_unit_t adc, adc_channel_t ch, adc_resolution_t res,
                   adc_atten_t atten, uint8_t sample_cycle)
{
    /* Configure ADC1 */
    if (adc == ADC_UNIT_1) {
        adc1_channel = ch;
        /* Configure SAR ADC1 bit width in START_FORCE register */
        uint32_t sf = SENS_SAR_START_FORCE_REG;
        sf &= ~SENS_SAR1_BIT_WIDTH_MASK;                     /* Clear bit width bits */
        sf |= ((uint32_t)res << SENS_SAR1_BIT_WIDTH_POS);    /* Set bit width */
        SENS_SAR_START_FORCE_REG = sf;

        /* Configure sample bit width and sample cycle in READ_CTRL register */
        uint32_t rc = SENS_SAR_READ_CTRL_REG;
        rc &= ~(SENS_SAR1_SAMPLE_BIT_MASK | SENS_SAR1_SAMPLE_CYCLE_MASK);
        rc |= ((uint32_t)res << SENS_SAR1_SAMPLE_BIT);
        rc |= ((uint32_t)(sample_cycle & 0xFF) << SENS_SAR1_SAMPLE_CYCLE);

        /* Ensure ADC1 is controlled by RTC (clear DIG_FORCE) */
        rc &= ~(1u << SENS_SAR1_DIG_FORCE);

        /* Set data inversion off (normal data) */
        rc |= (1u << SENS_SAR1_DATA_INV);

        SENS_SAR_READ_CTRL_REG = rc;

        /* Set attenuation for the selected channel */
        uint32_t att = SENS_SAR_ATTEN1_REG;
        att &= ~(0x3u << (ch * 2));          /* Clear attenuation bits for channel */
        att |= ((uint32_t)atten << (ch * 2)); /* Set attenuation */
        SENS_SAR_ATTEN1_REG = att;
    }
    /* Configure ADC2 */
    else if (adc == ADC_UNIT_2) {
        adc2_channel = ch;
        /* Configure SAR ADC2 bit width */
        uint32_t sf = SENS_SAR_START_FORCE_REG;
        sf &= ~SENS_SAR2_BIT_WIDTH_MASK;
        sf |= ((uint32_t)res << SENS_SAR2_BIT_WIDTH_POS);
        SENS_SAR_START_FORCE_REG = sf;

        /* Configure sample bit width and sample cycle in READ_CTRL2 register */
        uint32_t rc = SENS_SAR_READ_CTRL2_REG;
        rc &= ~(SENS_SAR2_SAMPLE_BIT_MASK | SENS_SAR2_SAMPLE_CYCLE_MASK);
        rc |= ((uint32_t)res << SENS_SAR2_SAMPLE_BIT);
        rc |= ((uint32_t)(sample_cycle & 0xFF) << SENS_SAR2_SAMPLE_CYCLE);

        /* Ensure ADC2 is controlled by RTC (clear DIG_FORCE) */
        rc &= ~(1u << SENS_SAR2_DIG_FORCE);

        /* Set data inversion off */
        rc |= (1u << SENS_SAR2_DATA_INV);

        SENS_SAR_READ_CTRL2_REG = rc;

        /* Set attenuation for selected channel */
        uint32_t att = SENS_SAR_ATTEN2_REG;
        att &= ~(0x3u << (ch * 2));
        att |= ((uint32_t)atten << (ch * 2));
        SENS_SAR_ATTEN2_REG = att;
    }
}

/**
 * @brief Read ADC conversion result from the configured channel of specified ADC unit.
 * 
 * @param adc  ADC unit to read from (ADC_UNIT_1 or ADC_UNIT_2)
 * @return uint16_t 12-bit ADC conversion result (0 - 4095)
 */
uint16_t adc_read(adc_unit_t adc)
{
    if (adc == ADC_UNIT_1) {
        /* Enable pad control: force enable and start measurement */
        SENS_SAR_MEAS_START1_REG |= (1u << SENS_SAR1_EN_PAD_FORCE) | (1u << SENS_MEAS1_START_FORCE);

        /* Select ADC1 channel by setting corresponding bit in EN_PAD */
        SENS_SAR_MEAS_START1_REG &= ~(((uint32_t)0xFFF) << SENS_SAR1_EN_PAD);
        SENS_SAR_MEAS_START1_REG |= (1u << (SENS_SAR1_EN_PAD + adc1_channel));

        /* Start ADC1 conversion by toggling START_SAR bit */
        SENS_SAR_MEAS_START1_REG &= ~(1u << SENS_MEAS1_START_SAR);
        SENS_SAR_MEAS_START1_REG |= (1u << SENS_MEAS1_START_SAR);

        /* Wait for conversion done flag */
        while ((SENS_SAR_MEAS_START1_REG & (1u << SENS_MEAS1_DONE_SAR)) == 0) { }

        /* Read 12-bit ADC1 conversion result */
        return (uint16_t)(SENS_SAR_MEAS_START1_REG & 0x0FFFu);
    }
    else {
        /* Enable pad control for ADC2 */
        SENS_SAR_MEAS_START2_REG |= (1u << SENS_SAR2_EN_PAD_FORCE) | (1u << SENS_MEAS2_START_FORCE);

        /* Select ADC2 channel */
        SENS_SAR_MEAS_START2_REG &= ~(((uint32_t)0xFFF) << SENS_SAR2_EN_PAD);
        SENS_SAR_MEAS_START2_REG |= (1u << (SENS_SAR2_EN_PAD + adc2_channel));

        /* Start ADC2 conversion */
        SENS_SAR_MEAS_START2_REG &= ~(1u << SENS_MEAS2_START_SAR);
        SENS_SAR_MEAS_START2_REG |= (1u << SENS_MEAS2_START_SAR);

        /* Wait for conversion done flag */
        while ((SENS_SAR_MEAS_START2_REG & (1u << SENS_MEAS2_DONE_SAR)) == 0) { }

        /* Read 12-bit ADC2 conversion result */
        return (uint16_t)(SENS_SAR_MEAS_START2_REG & 0x0FFFu);
    }
}
