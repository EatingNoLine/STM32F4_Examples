//
// Created by azarai on 8/10/24.
//

#ifndef BLDC_H
#define BLDC_H

#include "stm32f4xx_hal.h"

typedef struct {
  TIM_HandleTypeDef *timer;
  uint32_t timer_channel;
  double speed;
  uint32_t min_output;
  uint32_t max_output;
} BLDC_TypeDef;

void BLDC_Init(BLDC_TypeDef *bldc, TIM_HandleTypeDef *timer, uint32_t timer_channel, uint32_t min_output, uint32_t max_output);
void BLDC_SetSpeed(BLDC_TypeDef *bldc, double percentage);
void BLDC_Stop(BLDC_TypeDef *bldc);


#endif //BLDC_H
