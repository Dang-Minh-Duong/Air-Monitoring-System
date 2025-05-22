#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void timer_hw_init();
uint64_t timer_hw_get_count();
void delay_us_hw(uint32_t us);
void delay_ms_hw(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif 
