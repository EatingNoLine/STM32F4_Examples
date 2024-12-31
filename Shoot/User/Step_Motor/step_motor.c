//
// Created by azarai on 8/10/24.
//

#include "step_motor.h"

#include <stdio.h>

void SM_Init(SM_TypeDef *sm, TIM_HandleTypeDef *timer, uint32_t timer_channel,
             GPIO_TypeDef *enable_port, uint32_t enable_pin,
             GPIO_TypeDef *direction_port, uint32_t direction_pin) {
  // set up timer
  sm->timer = timer;
  sm->timer_channel = timer_channel;
  __HAL_TIM_SetCompare(timer, timer_channel, 500);   // ARR = 1000 - 1

  // set up default enable/directoin pin
  sm->enable_port = enable_port;
  sm->enable_pin = enable_pin;
  sm->direction_port = direction_port;
  sm->direction_pin = direction_pin;

  HAL_GPIO_WritePin(enable_port, enable_pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(direction_port, direction_pin, GPIO_PIN_SET);

  // set default speed
  SM_SetSpeed(sm, SM_SPEED_Slow);

  // set default count and target
  sm->count = 0;
  sm->target = 0;
}

void SM_SetDirection(SM_TypeDef *sm, SM_DIERCTION_State dir) {
  if (dir == SM_DIRECTION_FORWARD) {
    HAL_GPIO_WritePin(sm->direction_port, sm->direction_pin, GPIO_PIN_SET);
  } else if (dir == SM_DIRECTION_BACKWARD) {
    HAL_GPIO_WritePin(sm->direction_port, sm->direction_pin, GPIO_PIN_RESET);
  }
}

void SM_SetSpeed(SM_TypeDef *sm, SM_SPEED_State speed) {
  sm->speed = speed;

  // change timer's prescaler
  uint32_t psc = 0;
  switch (speed) {
    case SM_SPEED_Slow:
      psc = 143;  // 72MHz -> 100Hz
      break;
    case SM_SPEED_Medium:
      psc = 71;  // 72MHz -> 1KHz
      break;
    case SM_SPEED_Fast:
      psc = 3;  // 72MHz -> 18KHz
  }
  __HAL_TIM_SET_PRESCALER(sm->timer, psc);
}

void SM_Run(SM_TypeDef *sm, SM_SPEED_State speed, uint32_t step) {
  // set up the speed
  SM_SetSpeed(sm, speed);
  // start timer's count function
  sm->count = 0;
  sm->target = step;
  HAL_TIM_PWM_Start(sm->timer, sm->timer_channel);
}

void SM_UpdateCount(SM_TypeDef *sm) {
  printf("count = %d\n", sm->count);
  if (sm->count < sm->target) {
    ++sm->count;
  } else if (sm->count == sm->target) {
    sm->count = 0;
    sm->target = 0;
    SM_Stop(sm);
  }
}

void SM_Stop(SM_TypeDef *sm) {
  sm->count = 0;
  sm->target = 0;
  HAL_TIM_PWM_Stop(sm->timer, sm->timer_channel);
}
