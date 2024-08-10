//
// Created by azarai on 8/2/24.
//

#ifndef ARM_H
#define ARM_H

#include "stm32f4xx_hal.h"
#include "motor.h"

typedef struct {
    Motor_TypeDef *motor1;
    Motor_TypeDef *motor2;
} Arm_TypeDef;

void Arm_Init(Arm_TypeDef *arm, Motor_TypeDef *motor1, Motor_TypeDef *motor2);
void Arm_SetGesgure(Arm_TypeDef *arm, double degree1, double degree2);

#endif //ARM_H
