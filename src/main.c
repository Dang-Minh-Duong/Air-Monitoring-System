
#include "pwm.h"

void app_main(void) {
    uint8_t gpio = 18;
    uint8_t channel = 0;
    uint8_t timer = 0;
    uint32_t freq = 1000;       // 1kHz
    uint8_t resolution = 10;    // 10-bit
    uint32_t duty = 512;        // 50%

    pwm_init(gpio, channel, timer, freq, resolution, duty);
}

