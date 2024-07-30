//
// Created by azarai on 7/24/24.
//

#include "encoder.h"

void Encoder_Init(Encoder_Typedef *encoder) {
  // Initialize the timer
  HAL_TIM_Encoder_Start(encoder->timer, encoder->channels);
  // Initialize the encoder
  encoder->count = 0;
  encoder->is_initialized = true;
}

void Encoder_Update(Encoder_Typedef *encoder) {
  if (encoder->is_initialized) {
    encoder->count = __HAL_TIM_GetCounter(encoder->timer);
    __HAL_TIM_SetCounter(encoder->timer, 0);
  }
}

double Encoder_GetSpeed(const Encoder_Typedef *encoder) {
  return (double)encoder->count * encoder->frequency / encoder->ppr;
}