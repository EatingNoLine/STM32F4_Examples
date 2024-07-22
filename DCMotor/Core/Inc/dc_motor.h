//
// Created by azarai on 7/21/24.
//

#ifndef DC_MOTOR_H_H
#define DC_MOTOR_H_H

#include "main.h"

extern TIM_HandleTypeDef htim1;

enum MOTOR_State { MOTOR_RUNA, MOTOR_RUNB, MOTOR_STOP };

void MOTOR_Run1(void);

void MOTOR_Run2(void);

void MOTOR_Toggle(void);

void MOTOR_Stop(void);

void MOTOR_SetSpeed(uint32_t speed);

#endif  // DC_MOTOR_H_H
