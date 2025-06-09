#include <stdio.h>
#include "pwm.h"
#include "gpio_config.h"
#include "motor_control.h"
#include "ADC.h"
#include "led.h"
#include "i2c.h"
#include "lcd.h"
#include "timer.h"
#include "wifi.h"
#include "thingspeak.h"

/* PWM configuration */
#define PWM_TIMER 0      /* High-speed timer number */
#define PWM_CHANNEL 0    /* PWM channel number */
#define PWM_RES_BITS 8   /* PWM resolution in bits */
#define PWM_GPIO 18      /* GPIO number for PWM output */
#define PWM_FREQ_HZ 1000 /* PWM frequency in Hertz */

/*Thingspeak constant*/
#define THINGSPEAK_API_KEY "470C88FELGY34J82"
/**
 * @brief Converts an integer to a string.
 *
 * @param value     The integer value to convert.
 * @param buffer    Pointer to the character array where the resulting string will be stored.
 * @param buf_size  The size of the buffer in bytes.
 *
 * @return A pointer to the buffer containing the string representation of the integer.
 */
char *int_to_str(int value, char *buffer, size_t buf_size);

void app_main(void)
{
   /* Initialize PWM
      Parameters:
      - timer_num: timer group number (0 or 1)
      - channel: PWM channel (0 to 7)
      - resolution_bits: PWM resolution in bits (e.g., 8)
      - gpio_num: GPIO number to output PWM signal
      - freq_hz: PWM frequency in Hz
   */
   pwm_init(PWM_TIMER, PWM_CHANNEL, PWM_RES_BITS, PWM_GPIO, PWM_FREQ_HZ);

   /* Initialize timer
      Parameters:
      - timer_group: timer group number (0 or 1)
      - timer_num: timer number in group (0 or 1)
      - prescaler: clock prescaler value (e.g., 80)
   */
   Timer_Init(0, 1, 80);

   /* Initialize LCD
      Parameters:
      - sda_gpio: GPIO number for I2C SDA
      - scl_gpio: GPIO number for I2C SCL
   */
   lcd_init(21, 22);

   /* Turn on LCD backlight */
   lcd_backlight();

   /* Setup GPIO for ADC
      Parameters:
      - adc_unit: ADC unit (ADC_UNIT_1 or ADC_UNIT_2)
      - gpio_num: GPIO number connected to ADC channel
   */
   gpio_adc_setup(ADC_UNIT_1, 35);

   /* Configure ADC
      Parameters:
      - adc_unit: ADC unit (ADC_UNIT_1 or ADC_UNIT_2)
      - channel: ADC channel number
      - width: ADC resolution width (e.g., ADC_WIDTH_12Bit)
      - atten: input attenuation level (e.g., ADC_ATTEN_11DB)
      - sample_count: number of samples for averaging
   */
   adc_configure(ADC_UNIT_1, 7, ADC_WIDTH_12Bit, ADC_ATTEN_11DB, 10);

   /* Setup GPIOs for LEDs (GPIO numbers) */
   gpio_led_setup(25);
   gpio_led_setup(26);
   gpio_led_setup(27);

   /* Initialize WiFi station mode and connect
      Parameters:
      - ssid: WiFi network name
      - password: WiFi network password
   */
   wifi_init_sta("MinhDuong", "minhduong08");

   /* Delay for given timer group, timer, and milliseconds
      Parameters:
      - timer_group: timer group number
      - timer_num: timer number
      - delay_ms: delay time in milliseconds
   */
   Timer_Delay(0, 1, 5000);

   while (1)
   {
      uint16_t adcValue = adc_read(ADC_UNIT_1);
      char buffer[10];
      int_to_str(adcValue, buffer, sizeof(buffer));
      lcd_set_cursor(0, 0);
      lcd_print("PPM Value: ");
      lcd_print(buffer);

      lcd_set_cursor(0, 1);
      lcd_print("Current Speed Level: ");
      lcd_print_duty_level(PWM_CHANNEL, PWM_RES_BITS);

      /* Control LEDs based on ADC value and threshold parameters
         Parameters:
         - gpio_led1, gpio_led2, gpio_led3: GPIO numbers for LEDs
         - adcValue: current ADC reading
         - low_threshold: lower threshold value
         - high_threshold: higher threshold value
      */
      ledControl(21, 22, 23, adcValue, 500, 700);

      /* Send integer value to Blynk
         Parameters:
         - value: integer to send
         - auth_token: Blynk authorization token string
         - virtual_pin: Blynk virtual pin string
         - tag: log tag string
      */
      motor_control_auto(PWM_CHANNEL, PWM_RES_BITS, adcValue, 500, 700, 5, 300, 0, 1);
      send_to_thingspeak(THINGSPEAK_API_KEY, adcValue);
      Timer_Delay(0, 1, 500);
   }
}
char *int_to_str(int value, char *buffer, size_t buf_size)
{
   snprintf(buffer, buf_size, "%d", value);
   return buffer;
}
