#ifndef ESP32_PWM_REGS_H
#define ESP32_PWM_REGS_H

#include <stdint.h>

/**
 * @brief Peripheral clock enable register.
 * Used to enable clocks for various peripherals including LEDC (bit 11).
 */


/**
 * @brief LEDC high-speed timer configuration register.
 * @param n Timer number (0–3).
 */
#define LEDC_HSTIMER_CONF_REG(n) (*(volatile uint32_t *)(0x3FF59140 + (n) * 0x8))

/**
 * @brief LEDC high-speed channel configuration register 0.
 * Controls timer selection and output enable.
 * @param n Channel number (0–7).
 */
#define LEDC_HSCH_CONF0_REG(n) (*(volatile uint32_t *)(0x3FF59000 + (n) * 0x14))

/**
 * @brief LEDC high-speed channel high point register.
 * Defines the point in the PWM period when the output goes high.
 * @param n Channel number (0–7).
 */
#define LEDC_HSCH_HPOINT_REG(n) (*(volatile uint32_t *)(0x3FF59004 + (n) * 0x14))

/**
 * @brief LEDC high-speed channel duty cycle register.
 * Sets the duty of the PWM signal.
 * @param n Channel number (0–7).
 */
#define LEDC_HSCH_DUTY_REG(n) (*(volatile uint32_t *)(0x3FF59008 + (n) * 0x14))

/**
 * @brief LEDC high-speed channel configuration register 1.
 * Includes duty update enable and output idle level.
 * @param n Channel number (0–7).
 */
#define LEDC_HSCH_CONF1_REG(n) (*(volatile uint32_t *)(0x3FF5900C + (n) * 0x14))

#endif // ESP32_PWM_REGS_H