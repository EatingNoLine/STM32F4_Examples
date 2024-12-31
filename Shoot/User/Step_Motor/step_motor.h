//
// Created by azarai on 8/10/24.
//

#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

#include "stm32f4xx_hal.h"

typedef enum { SM_SPEED_Slow, SM_SPEED_Medium, SM_SPEED_Fast } SM_SPEED_State;

typedef enum { SM_DIRECTION_FORWARD, SM_DIRECTION_BACKWARD } SM_DIERCTION_State;

typedef struct {
  TIM_HandleTypeDef *timer;
  uint32_t timer_channel;

  GPIO_TypeDef *enable_port;
  uint32_t enable_pin;

  GPIO_TypeDef *direction_port;
  uint32_t direction_pin;

  SM_SPEED_State speed;
  uint32_t count;
  uint32_t target;
} SM_TypeDef;

void SM_Init(SM_TypeDef *sm, TIM_HandleTypeDef *timer, uint32_t timer_channel,
             GPIO_TypeDef *enable_port, uint32_t enable_pin,
             GPIO_TypeDef *direction_port, uint32_t direction_pin);
void SM_SetDirection(SM_TypeDef *sm, SM_DIERCTION_State dir);
void SM_SetSpeed(SM_TypeDef *sm, SM_SPEED_State speed);
void SM_Run(SM_TypeDef *sm, SM_SPEED_State speed, uint32_t step);
void SM_UpdateCount(SM_TypeDef *sm);
void SM_Stop(SM_TypeDef *sm);

#endif  // STEP_MOTOR_H
