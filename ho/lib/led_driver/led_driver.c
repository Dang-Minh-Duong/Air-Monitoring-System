#include "led_driver.h"

// Định nghĩa chân GPIO kết nối LED
#define LED_RED_PIN    15
#define LED_GREEN_PIN  2
#define LED_BLUE_PIN   4

void LED_Init(void) {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    // Tắt hết LED lúc khởi tạo
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}

void LED_Red_On(void) {
    digitalWrite(LED_RED_PIN, HIGH);
}

void LED_Red_Off(void) {
    digitalWrite(LED_RED_PIN, LOW);
}

void LED_Green_On(void) {
    digitalWrite(LED_GREEN_PIN, HIGH);
}

void LED_Green_Off(void) {
    digitalWrite(LED_GREEN_PIN, LOW);
}

void LED_Blue_On(void) {
    digitalWrite(LED_BLUE_PIN, HIGH);
}

void LED_Blue_Off(void) {
    digitalWrite(LED_BLUE_PIN, LOW);
}
