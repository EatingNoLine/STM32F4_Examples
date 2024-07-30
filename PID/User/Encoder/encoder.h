//
// Created by azarai on 7/24/24.
//

#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef struct Encoder_Typedef{
  TIM_HandleTypeDef *timer;
  uint32_t channels;
  uint32_t ppr;
  uint32_t count;
  uint32_t frequency;
  bool is_initialized;
} Encoder_Typedef;

void Encoder_Init(Encoder_Typedef *encoder);
void Encoder_Update(Encoder_Typedef *encoder);
double Encoder_GetSpeed(const Encoder_Typedef *encoder);

#endif //ENCODER_H
