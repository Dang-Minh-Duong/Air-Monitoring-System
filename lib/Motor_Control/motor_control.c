#include "motor_control.h"

void motor_control_auto(uint8_t pwm_channel,
                        uint8_t resolution_bits,
                        uint16_t value,
                        float thres1,
                        float thres2,
                        float ramp_step,
                        uint32_t total_ramp_time_ms, int groupTimer, int timer)
{
    float target_duty = 0.0f;

    /*Determine target duty based on PPM thresholds*/
    if (value < thres1)
    {
        target_duty = 0.0f;
    }
    else if (value < thres2)
    {
        target_duty = 75.0f;
    }

    else
    {
        target_duty = 100.0f;
    }

    /*Use your existing ramp function for smooth transition*/
    motor_set_speed_ramp(pwm_channel, resolution_bits, target_duty, ramp_step, total_ramp_time_ms, groupTimer, timer);
}
void motor_set_speed_ramp(uint8_t pwm_channel,
                          uint8_t resolution_bits,
                          float target_percent,
                          float step_percent,
                          uint32_t total_ramp_time_ms, int groupTimer, int timer)
{
    /* Validate step percentage */
    if (step_percent <= 0.0f || step_percent > 100.0f)
        return;

    /* Get current duty cycle and determine direction */
    float current_percent = pwm_get_duty_percent(pwm_channel, resolution_bits);
    float step = (target_percent > current_percent) ? step_percent : -step_percent;
    float value = current_percent;

    /* Calculate automatic timing */
    float percent_diff = fabsf(target_percent - current_percent);
    uint32_t num_steps = (uint32_t)(percent_diff / step_percent);

    /* Prevent division by zero when no ramp needed */
    uint32_t delay_ms = (num_steps > 0) ? (total_ramp_time_ms / num_steps) : 0;

    /* Ensure minimum delay for system stability */
    if (delay_ms < 1)
        delay_ms = 1;

    /* Execute ramp */
    while (fabsf(value - target_percent) > step_percent)
    {
        value += step;
        /* Clamp value to valid range */
        value = (value < 0.0f) ? 0.0f : (value > 100.0f) ? 100.0f
                                                         : value;

        pwm_set_duty_percent(pwm_channel, resolution_bits, value);
        Timer_Delay(groupTimer, timer, delay_ms);
    }

    /* Ensure precise final value */
    pwm_set_duty_percent(pwm_channel, resolution_bits, target_percent);
}