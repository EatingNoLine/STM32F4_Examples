//
// Created by azarai on 7/23/24.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx_hal.h"

typedef enum Motor_RunDirection { Forward, Backward } Motor_RunDirection;

typedef struct Motor_TypeDef {
  TIM_HandleTypeDef *       pwm_timer;
  uint32_t                  pwm_channel;
  GPIO_TypeDef *            ina_port;
  uint32_t                  ina_pin;
  GPIO_TypeDef *            inb_port;
  uint32_t                  inb_pin;

  Motor_RunDirection        direction;
  uint32_t                  output;
  uint32_t                  ratio;
} Motor_TypeDef;

void Motor_Init(Motor_TypeDef *hMotor);

void Motor_SetDirection(Motor_TypeDef *hMotor, Motor_RunDirection direction);

void Motor_SetOutput(Motor_TypeDef *hMotor, uint32_t output);

void Motor_Run(const Motor_TypeDef *hMotor);

void Motor_Stop(Motor_TypeDef *hMotor);

#endif  // MOTOR_H
