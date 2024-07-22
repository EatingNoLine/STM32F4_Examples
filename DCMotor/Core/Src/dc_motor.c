//
// Created by azarai on 7/21/24.
//

#include "dc_motor.h"

void MOTOR_Run1() {
  HAL_GPIO_WritePin(SIGN_A_GPIO_Port, SIGN_A_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SIGN_B_GPIO_Port, SIGN_B_Pin, GPIO_PIN_RESET);
}

void MOTOR_Run2() {
  HAL_GPIO_WritePin(SIGN_A_GPIO_Port, SIGN_A_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SIGN_B_GPIO_Port, SIGN_B_Pin, GPIO_PIN_SET);
}

void MOTOR_Toggle() {
  HAL_GPIO_TogglePin(SIGN_A_GPIO_Port, SIGN_A_Pin);
  HAL_GPIO_TogglePin(SIGN_B_GPIO_Port, SIGN_B_Pin);
}

void MOTOR_Stop() {
  HAL_GPIO_WritePin(SIGN_A_GPIO_Port, SIGN_A_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SIGN_B_GPIO_Port, SIGN_B_Pin, GPIO_PIN_RESET);
}

void MOTOR_SetSpeed(uint32_t speed) {
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed * 10 - 1);
}