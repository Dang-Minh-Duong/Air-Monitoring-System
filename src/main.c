#include <Arduino.h>
#include "timer_module.h"  

void setup() {
  /* Initialize serial communication at 115200 baud */
  Serial.begin(115200);

  pinMode(4, OUTPUT);  

  Timer_Init(0, 0, 80, 500000, TIMER_INT_LEVEL);
  Timer_Isr_Register(0, 0, TIMER_INT_LEVEL);  /* Register interrupt handler */

}

bool state = 0; 

void loop() {
  /* Check if the timer ISR flag is set (Timer Group 1, Timer 0 → index 0) */
  if (timer_isr_triggered[0]) {
    state = ~state;
    digitalWrite(4, state);          
    Serial.println("Interrupt triggered");
    timer_isr_triggered[0] = 0;    
  }
}
