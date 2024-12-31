//
// Created by azarai on 29/09/24.
//

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "main.h"

#define MOTOR_MAX_OUTPUT 999
#define MOTOR_REDUCTION_RATIO 27
#define MOTOR_UPDATE_FREQUENCY 10

typedef struct _DM {
  // timer
  TIM_HandleTypeDef *timer;
  uint32_t channel;
  // input a
  GPIO_TypeDef *port_a;
  uint32_t pin_a;
  // input b
  GPIO_TypeDef *port_b;
  uint32_t pin_b;
  // basic data
  double speed;
} DM;

typedef enum _DM_Direction {
  DM_DIR_Forward,
  DM_DIR_Backward,
  DM_DIR_Stop
} DM_Direction;

// Functions

// Motor Section
void DM_Init(void);
void DM_SetSpeed(int i, double speed);
void DM_SetDirection(int i, DM_Direction dir);
void DM_SetOutput(int i, int32_t output);
void DM_UpdateOutput(int i);

// Encoder Section
#define ENCODER_BASE_COUNT 5000
#define ENCODER_MULTIPLE 4
#define ENCODER_PPR 17

typedef struct _Encoder {
  TIM_HandleTypeDef *timer;
  uint32_t channel;
} Encoder;

void Encoder_Init(void);
int32_t Encoder_GetCount(int i);
double Encoder_GetSpeed(int i);

// PID Section

typedef struct _PID {
  // PID parameter
  double Kp;
  double Ki;
  double Kd;
  // Errors : e[0] = e(k), e[1] = e(k-1), e[2] = e(k-2)
  double error[3];
  // Threshold
  double max_output;
  double max_integral;
  // Output
  double output;
} PID;

void PID_Init(void);
double PID_Increase_Calc(int i, double target, double actual);

// Extern global variables
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4, htim5, htim9, htim14;
// Global variables
extern DM dm[4];
extern Encoder enc[4];
extern PID pid[4];

#endif  // MOTOR_CONTROL_H
