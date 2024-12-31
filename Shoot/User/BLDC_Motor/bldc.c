//
// Created by azarai on 8/10/24.
//

#include "bldc.h"

void BLDC_Init(BLDC_TypeDef *bldc, TIM_HandleTypeDef *timer,
               uint32_t timer_channel, uint32_t min_output,
               uint32_t max_output) {
  // set up threshold value
  bldc->min_output = min_output;
  bldc->max_output = max_output;

  // set up speed
  bldc->speed = 0;

  // set up timer
  bldc->timer = timer;
  bldc->timer_channel = timer_channel;
  __HAL_TIM_SetCompare(timer, timer_channel, min_output);
  HAL_TIM_PWM_Start(timer, timer_channel);
}

void BLDC_SetSpeed(BLDC_TypeDef *bldc, double percentage) {
  if (bldc->speed != 0 && percentage == 0) {
    BLDC_Stop(bldc);
  } else {
      uint32_t output = bldc->min_output + (uint32_t)(percentage * (bldc->max_output - bldc->min_output));
      __HAL_TIM_SetCompare(bldc->timer, bldc->timer_channel, output);

    // re-start Timer if BLDC is closed before
    if (bldc->speed == 0) {
      HAL_TIM_PWM_Start(bldc->timer, bldc->timer_channel);
    }

    bldc->speed = percentage;
  }
}

void BLDC_Stop(BLDC_TypeDef *bldc) {
  bldc->speed = 0;
  HAL_TIM_PWM_Stop(bldc->timer, bldc->timer_channel);
}