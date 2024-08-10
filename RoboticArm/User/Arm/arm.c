//
// Created by azarai on 8/2/24.
//

#include "arm.h"

void Arm_Init(Arm_TypeDef *arm, Motor_TypeDef *motor1, Motor_TypeDef *motor2) {
    arm->motor1 = motor1;
    arm->motor2 = motor2;
}

void Arm_SetGesgure(Arm_TypeDef *arm, double degree1, double degree2) {
    Motor_Rotate(arm->motor1, degree1);
    Motor_Rotate(arm->motor2, degree2);
}
