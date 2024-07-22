//
// Created by azarai on 7/21/24.
//

#ifndef KEY_H
#define KEY_H

#include "dc_motor.h"
#include "main.h"

extern uint32_t speed;
extern TIM_HandleTypeDef htim1;

extern enum MOTOR_State motor_state;

void KEY_Event_Left(void);
void KEY_Event_Right(void);
void KEY_Event_Toggle(void);

#endif  // KEY_H
