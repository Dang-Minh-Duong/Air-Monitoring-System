#include "motor_control.h"

/*
 * @brief Automatically control motor speed based on sensor value thresholds.
 *
 * This function determines the target motor speed duty cycle percentage
 * based on two threshold values (thres1 and thres2). It sets the duty cycle
 * to 0%, 75%, or 100% depending on the value range.
 * Then, it calls the ramp function to smoothly transition to the target speed.
 *
 * @param pwm_channel PWM channel number controlling the motor.
 * @param resolution_bits Resolution bits used for PWM duty cycle.
 * @param value Current sensor reading (e.g. PPM value).
 * @param thres1 Lower threshold for value.
 * @param thres2 Upper threshold for value.
 * @param ramp_step Step size (percentage) for duty cycle ramping.
 * @param total_ramp_time_ms Total time in milliseconds for the ramp transition.
 * @param groupTimer Timer group identifier for delay function.
 * @param timer Timer identifier for delay function.
 */
void motor_control_auto(uint8_t pwm_channel,
                        uint8_t resolution_bits,
                        uint16_t value,
                        float thres1,
                        float thres2,
                        float ramp_step,
                        uint32_t total_ramp_time_ms, int groupTimer, int timer)
{
    float target_duty = 0.0f;

    /* Determine target duty based on thresholds */
    if (value < thres1)
    {
        target_duty = 0.0f;        /* Below lower threshold -> motor off */
    }
    else if (value < thres2)
    {
        target_duty = 75.0f;       /* Between thresholds -> medium speed */
    }
    else
    {
        target_duty = 100.0f;      /* Above upper threshold -> full speed */
    }

    /* Smoothly ramp motor speed to target duty */
    motor_set_speed_ramp(pwm_channel, resolution_bits, target_duty, ramp_step, total_ramp_time_ms, groupTimer, timer);
}

/*
 * @brief Ramp motor speed duty cycle smoothly to a target percentage.
 *
 * This function gradually changes the PWM duty cycle from the current
 * duty cycle to the target_percent, incrementing/decrementing by step_percent.
 * The total ramp duration is divided among the steps with delays in between.
 *
 * @param pwm_channel PWM channel number controlling the motor.
 * @param resolution_bits Resolution bits used for PWM duty cycle.
 * @param target_percent Target PWM duty cycle percentage (0-100%).
 * @param step_percent Step size percentage to increment/decrement duty cycle.
 * @param total_ramp_time_ms Total ramp duration in milliseconds.
 * @param groupTimer Timer group identifier for delay function.
 * @param timer Timer identifier for delay function.
 */
void motor_set_speed_ramp(uint8_t pwm_channel,
                          uint8_t resolution_bits,
                          float target_percent,
                          float step_percent,
                          uint32_t total_ramp_time_ms, int groupTimer, int timer)
{
    /* Validate step percentage to avoid invalid ramping */
    if (step_percent <= 0.0f || step_percent > 100.0f)
        return;

    /* Get current PWM duty cycle percentage */
    float current_percent = pwm_get_duty_percent(pwm_channel, resolution_bits);

    /* Determine ramp direction: positive or negative step */
    float step = (target_percent > current_percent) ? step_percent : -step_percent;
    float value = current_percent;

    /* Calculate difference and number of steps needed */
    float percent_diff = fabsf(target_percent - current_percent);
    uint32_t num_steps = (uint32_t)(percent_diff / step_percent);

    /* Calculate delay per step to distribute total ramp time */
    uint32_t delay_ms = (num_steps > 0) ? (total_ramp_time_ms / num_steps) : 0;

    /* Ensure at least 1 ms delay for system stability */
    if (delay_ms < 1)
        delay_ms = 1;

    /* Perform the ramping loop */
    while (fabsf(value - target_percent) > step_percent)
    {
        value += step;

        /* Clamp value between 0 and 100 */
        if (value < 0.0f)
            value = 0.0f;
        else if (value > 100.0f)
            value = 100.0f;

        /* Set PWM duty cycle */
        pwm_set_duty_percent(pwm_channel, resolution_bits, value);

        /* Wait between steps */
        Timer_Delay(groupTimer, timer, delay_ms);
    }

    /* Ensure final precise duty cycle set */
    pwm_set_duty_percent(pwm_channel, resolution_bits, target_percent);
}
