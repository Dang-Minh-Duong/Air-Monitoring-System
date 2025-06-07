#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void lcd_init(void);
void lcd_backlight(void);
void lcd_send_command(uint8_t command);
void lcd_send_data(uint8_t data);
void lcd_print(const char *str);
void lcd_set_cursor(uint8_t col, uint8_t row);

#endif // LCD_H
