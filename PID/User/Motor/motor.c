//
// Created by azarai on 7/23/24.
//

#include "motor.h"

void Motor_Init(Motor_TypeDef *hMotor) {
  // Initialize the timer
  HAL_TIM_PWM_Start(hMotor->pwm_timer, hMotor->pwm_channel);
  // Initialize the motor
  hMotor->direction = Forward;
  hMotor->output = 0;
}

void Motor_SetDirection(Motor_TypeDef *hMotor, Motor_RunDirection direction) {
  hMotor->direction = direction;
}

void Motor_SetOutput(Motor_TypeDef *hMotor, uint32_t output) {
  hMotor->output = output;
  __HAL_TIM_SetCompare(hMotor->pwm_timer,
                       hMotor->pwm_channel, hMotor->output);
}

void Motor_Run(const Motor_TypeDef *hMotor) {
  if (hMotor->direction == Forward) {
    HAL_GPIO_WritePin(hMotor->ina_port,
                      hMotor->ina_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hMotor->inb_port,
                      hMotor->inb_pin, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(hMotor->ina_port,
                      hMotor->ina_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hMotor->inb_port,
                      hMotor->inb_pin, GPIO_PIN_SET);
  }
}

void Motor_Stop(Motor_TypeDef *hMotor) {
  hMotor->output = 0;
  HAL_GPIO_WritePin(hMotor->ina_port,
                    hMotor->ina_pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(hMotor->inb_port,
                    hMotor->inb_pin, GPIO_PIN_RESET);
}
