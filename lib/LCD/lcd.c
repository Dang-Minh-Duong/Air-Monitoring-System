#include "lcd.h"
#include "i2c.h"

#define LCD_BACKLIGHT   0x08
#define LCD_ENABLE_BIT  0x04
#define LCD_LINES       4

static void delay(int count) {
    while (count--) {
        asm volatile ("nop");
    }
}

static void lcd_expanderWrite(uint8_t data) {
    uint8_t payload[1] = {data | LCD_BACKLIGHT};
    i2c_master_write_byte_new(payload, 1);
}

static void lcd_pulseEnable(uint8_t data) {
    lcd_expanderWrite(data | LCD_ENABLE_BIT);
    delay(1);
    lcd_expanderWrite(data & ~LCD_ENABLE_BIT);
    delay(5);
}

static void lcd_write4Bits(uint8_t value) {
    lcd_expanderWrite(value);
    lcd_pulseEnable(value);
}

static void lcd_send(uint8_t value, uint8_t mode) {
    uint8_t highNibble = value & 0xF0;
    uint8_t lowNibble = (value << 4) & 0xF0;
    lcd_write4Bits(highNibble | mode);
    lcd_write4Bits(lowNibble | mode);
}

void lcd_send_command(uint8_t command) {
    lcd_send(command, 0);
}

void lcd_send_data(uint8_t data) {
    lcd_send(data, 0x01);
}

void lcd_backlight(void) {
    lcd_expanderWrite(LCD_BACKLIGHT);
}

void lcd_init(void) {
    i2c_init();
    delay(50000);
    lcd_send_command(0x33);
    lcd_send_command(0x32);
    lcd_send_command(0x28);
    lcd_send_command(0x0C);
    lcd_send_command(0x06);
    lcd_send_command(0x01);
    delay(2000);
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row >= LCD_LINES) row = LCD_LINES - 1;
    lcd_send_command(0x80 | (col + row_offsets[row]));
}
