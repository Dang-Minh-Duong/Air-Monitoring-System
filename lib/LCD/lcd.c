#include "lcd.h"
#include "i2c.h"
#include "timer.h"
#include <stdio.h>

/**
 * @brief Write a byte to the LCD expander via I2C, keeping backlight on.
 *
 * @param data Byte to write to the expander.
 */
static void lcd_expanderWrite(uint8_t data)
{
    uint8_t payload[1] = {data | LCD_BACKLIGHT};
    i2c_master_write_byte_new(payload, 1);
}

/**
 * @brief Pulse the LCD enable bit to latch data.
 *
 * @param data Byte with data and control bits to send.
 */
static void lcd_pulseEnable(uint8_t data)
{
    lcd_expanderWrite(data | LCD_ENABLE_BIT);
    Timer_Delay(0, 1, 1); /* Short delay */
    lcd_expanderWrite(data & ~LCD_ENABLE_BIT);
    Timer_Delay(0, 1, 5); /* Wait for LCD to process */
}

/**
 * @brief Write 4 bits (half byte) to LCD.
 *
 * @param value Upper nibble contains data and control bits.
 */
static void lcd_write4Bits(uint8_t value)
{
    lcd_expanderWrite(value);
    lcd_pulseEnable(value);
}

/**
 * @brief Send a full byte to LCD in 4-bit mode.
 *
 * @param value Byte data or command to send.
 * @param mode Control bit: 0 for command, 1 for data.
 */
static void lcd_send(uint8_t value, uint8_t mode)
{
    uint8_t highNibble = value & 0xF0;
    uint8_t lowNibble = (value << 4) & 0xF0;
    lcd_write4Bits(highNibble | mode);
    lcd_write4Bits(lowNibble | mode);
}

/**
 * @brief Send command byte to LCD.
 *
 * @param command Command byte to send.
 */
void lcd_send_command(uint8_t command)
{
    lcd_send(command, 0);
}

/**
 * @brief Send data byte (character) to LCD.
 *
 * @param data Data byte to send.
 */
void lcd_send_data(uint8_t data)
{
    lcd_send(data, 0x01);
}

/**
 * @brief Turn on LCD backlight.
 */
void lcd_backlight(void)
{
    lcd_expanderWrite(LCD_BACKLIGHT);
}

/**
 * @brief Initialize LCD display with given I2C pins.
 *
 * @param sda GPIO number for SDA line.
 * @param scl GPIO number for SCL line.
 */
void lcd_init(int sda, int scl)
{
    i2c_init(sda, scl);
    Timer_Delay(0, 1, 50000); /* Wait for LCD power up */

    lcd_send_command(0x33); /* Initialization sequence */
    lcd_send_command(0x32);
    lcd_send_command(0x28); /* 4-bit mode, 2 lines */
    lcd_send_command(0x0C); /* Display ON, cursor OFF */
    lcd_send_command(0x06); /* Entry mode set */
    lcd_send_command(0x01); /* Clear display */

    Timer_Delay(0, 1, 2000); /* Wait for clear */
}

/**
 * @brief Print a null-terminated string to the LCD.
 *
 * @param str Pointer to the string to print.
 */
void lcd_print(const char *str)
{
    while (*str)
    {
        lcd_send_data(*str++);
    }
}

/**
 * @brief Set cursor position on LCD.
 *
 * @param col Column index (0-based).
 * @param row Row index (0-based).
 */
void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row >= LCD_LINES)
        row = LCD_LINES - 1;
    lcd_send_command(0x80 | (col + row_offsets[row]));
}

/**
 * @brief Convert PWM duty level to string and print on LCD.
 *
 * @param channel_num PWM channel number.
 * @param resolution_bits Number of bits used in PWM resolution.
 */
void lcd_print_duty_level(uint8_t channel_num, uint8_t resolution_bits)
{
    uint8_t level = pwm_get_duty_level(channel_num, resolution_bits);

    char buffer[10];              /* buffer to hold converted number */
    sprintf(buffer, "%u", level); /* convert level to string */
    lcd_print(buffer);            /* print string on LCD */
}
