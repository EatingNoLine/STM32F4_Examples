//
// Created by azarai on 7/21/24.
//

#include "key.h"

#include <stm32f4xx_hal_tim.h>

void KEY_Event_Left(void) {
  if (speed > 0) {
    speed -= 20;
    MOTOR_SetSpeed(speed);
  }
}

void KEY_Event_Right(void) {
  if (speed < 100) {
    speed += 20;
    MOTOR_SetSpeed(speed);
  }
}

void KEY_Event_Toggle(void) {
  if (motor_state == MOTOR_STOP) {
    motor_state = MOTOR_RUNA;
    MOTOR_Run1();
  } else if (motor_state == MOTOR_RUNA) {
    motor_state = MOTOR_RUNB;
    MOTOR_Run2();
  } else if (motor_state == MOTOR_RUNB) {
    motor_state = MOTOR_STOP;
    MOTOR_Stop();
  }
}
