//
// Created by azarai on 7/21/24.
//

#include "beep.h"

void BEEP_On()  {
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
}

void BEEP_Off()  {
  HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);
}

void BEEP_Last(uint32_t time)  {
  BEEP_On();
  HAL_Delay(time);
  BEEP_Off();
}
