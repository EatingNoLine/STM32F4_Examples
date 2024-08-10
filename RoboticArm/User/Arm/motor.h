//
// Created by azarai on 8/2/24.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *timer;
    uint32_t timer_channel;
    double degree;
    double max_degree;
    double min_degree;
} Motor_TypeDef;

void Motor_Init(Motor_TypeDef *motor, TIM_HandleTypeDef *timer, uint32_t channel, double default_degree, double min_degree, double max_degree);
void Motor_Rotate(Motor_TypeDef *motor, double degree);


#endif //MOTOR_H
