//
// Created by azarai on 8/2/24.
//

#include "motor.h"

void Motor_Init(Motor_TypeDef *motor, TIM_HandleTypeDef *timer, uint32_t channel, double default_degree,
                double min_degree, double max_degree) {
    // Set up the motor instance
    motor->timer = timer;
    motor->timer_channel = channel;
    // Set motor's degree boundery
    motor->max_degree = max_degree;
    motor->min_degree = min_degree;
    // Set the degree to default degree
    motor->degree = default_degree;
    Motor_Rotate(motor, default_degree);
    // Activate the timer
    HAL_TIM_PWM_Start(timer, channel);
}

void Motor_Rotate(Motor_TypeDef *motor, double degree) {
    if (degree >= motor->min_degree && degree <= motor->max_degree) {
        motor->degree = degree;
        __HAL_TIM_SetCompare(motor->timer, motor->timer_channel, (uint16_t)(motor->degree/90 * 100)+100);
    }
}
