#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "i2c.h"
#include "timer.h"
#include <stdio.h>

/* LCD control bits */
#define LCD_BACKLIGHT 0x08  /* Backlight bit */
#define LCD_ENABLE_BIT 0x04 /* Enable bit */

/* Number of display lines (usually 2 or 4) */
#define LCD_LINES 2

/**
 * @brief Initialize LCD display via I2C with specified SDA and SCL GPIO pins.
 *
 * @param sda GPIO number for SDA line.
 * @param scl GPIO number for SCL line.
 */
void lcd_init(int sda, int scl);

/**
 * @brief Send a command byte to the LCD.
 *
 * @param command Command byte to send.
 */
void lcd_send_command(uint8_t command);

/**
 * @brief Send a data byte (character) to the LCD.
 *
 * @param data Data byte to send.
 */
void lcd_send_data(uint8_t data);

/**
 * @brief Turn on LCD backlight.
 */
void lcd_backlight(void);

/**
 * @brief Print a null-terminated string to the LCD.
 *
 * @param str Pointer to the string to print.
 */
void lcd_print(const char *str);

/**
 * @brief Set cursor position on LCD.
 *
 * @param col Column index (0-based).
 * @param row Row index (0-based).
 */
void lcd_set_cursor(uint8_t col, uint8_t row);

/**
 * @brief Convert PWM duty level to string and print on LCD.
 *
 * @param channel_num PWM channel number.
 * @param resolution_bits Number of bits used in PWM resolution.
 */
void lcd_print_duty_level(uint8_t channel_num, uint8_t resolution_bits);

#endif /* LCD_H */
