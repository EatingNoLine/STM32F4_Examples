//
// Created by azarai on 30/09/24.
//

#include "vofa.h"
#include "motor_control.h"

#include <stdio.h>


void AdjustPID(char *str) {
  char c;
  int tmp1, tmp2;
  sscanf(str, "%c=%d.%d", &c, &tmp1, &tmp2);
  double f = tmp1 + (double)tmp2 * 0.001;

  switch (c) {
    case 'P':
      pid[ii].Kp = f;
      break;
    case 'I':
      pid[ii].Ki = f;
      break;
    case 'D':
      pid[ii].Kd = f;
      break;
    case 'T':
      dm[ii].speed = f;
      break;
    default:
      break;
  }
}
