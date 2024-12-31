//
// Created by azarai on 29/09/24.
//

#include "motor_control.h"

#include <stdio.h>
#include <string.h>

// Global Variables
DM dm[4];
Encoder enc[4];
PID pid[4];
double pid_table[4][3] = {
    {0.5, 1.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 1.0, 0.5}};

// Functions' implements
void DM_Init(void) {
  // Initialize timer
  dm[0].timer = &htim9;
  dm[1].timer = &htim9;
  dm[2].timer = &htim5;
  dm[3].timer = &htim5;

  dm[0].channel = TIM_CHANNEL_1;
  dm[1].channel = TIM_CHANNEL_2;
  dm[2].channel = TIM_CHANNEL_3;
  dm[3].channel = TIM_CHANNEL_4;

  // Initialize input pin a
  dm[0].port_a = DM_INA1_GPIO_Port;
  dm[1].port_a = DM_INA2_GPIO_Port;
  dm[2].port_a = DM_INA3_GPIO_Port;
  dm[3].port_a = DM_INA4_GPIO_Port;

  dm[0].pin_a = DM_INA1_Pin;
  dm[1].pin_a = DM_INA2_Pin;
  dm[2].pin_a = DM_INA3_Pin;
  dm[3].pin_a = DM_INA4_Pin;

  // Initialize input pin b
  dm[0].port_b = DM_INB1_GPIO_Port;
  dm[1].port_b = DM_INB2_GPIO_Port;
  dm[2].port_b = DM_INB3_GPIO_Port;
  dm[3].port_b = DM_INB4_GPIO_Port;

  dm[0].pin_b = DM_INB1_Pin;
  dm[1].pin_b = DM_INB2_Pin;
  dm[2].pin_b = DM_INB3_Pin;
  dm[3].pin_b = DM_INB4_Pin;

  for (int i = 0; i < 4; ++i) {
    // Start timers and set up input pins to default mode
    __HAL_TIM_SetCompare(dm[i].timer, dm[i].channel, 0);
    HAL_TIM_PWM_Start(dm[i].timer, dm[i].channel);
    HAL_GPIO_WritePin(dm[i].port_a, dm[i].pin_a, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(dm[i].port_b, dm[i].pin_b, GPIO_PIN_RESET);
  }
}

void DM_SetSpeed(int i, double speed) {
  // Speed of motor is actually controlled by pid, so we just need to set target
  // value
  dm[i].speed = speed;
}

void DM_SetDirection(int i, DM_Direction dir) {
  if (dir == DM_DIR_Forward) {
    HAL_GPIO_WritePin(dm[i].port_a, dm[i].pin_a, GPIO_PIN_SET);
    HAL_GPIO_WritePin(dm[i].port_b, dm[i].pin_b, GPIO_PIN_RESET);
  } else if (dir == DM_DIR_Backward) {
    HAL_GPIO_WritePin(dm[i].port_a, dm[i].pin_a, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(dm[i].port_b, dm[i].pin_b, GPIO_PIN_SET);
  } else if (dir == DM_DIR_Stop) {
    HAL_GPIO_WritePin(dm[i].port_a, dm[i].pin_a, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(dm[i].port_b, dm[i].pin_b, GPIO_PIN_RESET);
  }
}

void DM_SetOutput(int i, int32_t output) {
  // Limit the output
  if (output > MOTOR_MAX_OUTPUT) output = MOTOR_MAX_OUTPUT;
  if (output < -MOTOR_MAX_OUTPUT) output = -MOTOR_MAX_OUTPUT;

  if (output > 0) {
    DM_SetDirection(i, DM_DIR_Forward);
    __HAL_TIM_SetCompare(dm[i].timer, dm[i].channel, output);
  } else if (output < 0) {
    DM_SetDirection(i, DM_DIR_Backward);
    __HAL_TIM_SetCompare(dm[i].timer, dm[i].channel, -output);
  } else {
    DM_SetDirection(i, DM_DIR_Stop);
  }
}

void DM_UpdateOutput(int i) {
  double speed = Encoder_GetSpeed(i);
  int32_t output = PID_Increase_Calc(i, dm[i].speed, speed);
  DM_SetOutput(i, output);

  // printf("Speed: %.3lf, %.3lf\n", dm[i].speed, speed);

}

void Encoder_Init(void) {
  // Set timers
  enc[0].timer = &htim1;
  enc[1].timer = &htim2;
  enc[2].timer = &htim3;
  enc[3].timer = &htim4;

  for (int i = 0; i < 4; ++i) {
    // Set channel of timers
    enc[i].channel = TIM_CHANNEL_1 | TIM_CHANNEL_2;

    // Start encoder timer
    __HAL_TIM_SetCounter(enc[i].timer, 0);
    HAL_TIM_Encoder_Start(enc[i].timer, enc[i].channel);
  }
}

int32_t Encoder_GetCount(int i) {
  int32_t count =
      (int32_t)(__HAL_TIM_GetCounter(enc[i].timer) - ENCODER_BASE_COUNT) /
      ENCODER_MULTIPLE;
  __HAL_TIM_SetCounter(enc[i].timer,
                       ENCODER_BASE_COUNT);  // Clear the value of counter
  return count;
}

double Encoder_GetSpeed(int i) {
  int32_t count = Encoder_GetCount(i);
  printf("count = %ld\n", count);
  return (double)count / (MOTOR_REDUCTION_RATIO * ENCODER_PPR) *
         MOTOR_UPDATE_FREQUENCY;
}

void PID_Init(void) {
  for (int i = 0; i < 4; ++i) {
    // PID constants
    pid[i].Kp = pid_table[i][0];
    pid[i].Ki = pid_table[i][1];
    pid[i].Kd = pid_table[i][2];
    // Output
    pid[i].output = 0;
    // Max numbers
    pid[i].max_output = MOTOR_MAX_OUTPUT;
    pid[i].max_integral = MOTOR_MAX_OUTPUT;
    // Errors
    memset(pid[i].error, 0, sizeof(double) * 3);
  }
  // Start update timer
  HAL_TIM_Base_Start_IT(&htim14);
}

double PID_Increase_Calc(int i, double target, double actual) {
  pid[i].error[2] = pid[i].error[1];
  pid[i].error[1] = pid[i].error[0];
  pid[i].error[0] = target - actual;

  if (pid[i].error[0] != 0) {
    pid[i].output +=
        pid[i].Kp * (pid[i].error[0] - pid[i].error[1]) +
        pid[i].Ki * pid[i].error[0] +
        pid[i].Kd * (pid[i].error[0] - 2 * pid[i].error[1] + pid[i].error[2]);
  }
  // Limit output
  if (pid[i].output > pid[i].max_output) pid[i].output = pid[i].max_output;
  if (pid[i].output < -pid[i].max_output) pid[i].output = -pid[i].max_output;

  return pid[i].output;
}
