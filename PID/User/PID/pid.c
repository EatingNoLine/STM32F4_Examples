//
// Created by azarai on 7/24/24.
//

#include "pid.h"

double abs_limit(double value, const double max) {
  if (value > max) value = max;
  if (value < -max) value = -max;
  return value;
}

void PID_Init(PID_TypeDef *pid, const double Kp, const double Ki,
              const double Kd, const double limit, const uint32_t target) {
  // Initialize the pid constant
  pid->Kp = Kp;
  pid->Ki = Ki;
  pid->Kd = Kd;
  // Initialize the errors
  pid->error[2] = 0;
  pid->error[1] = 0;
  pid->error[0] = 0;
  pid->error_sum = 0;
  pid->error_diff = 0;
  // Initialize the limit
  pid->integral_max = limit;
  pid->output_max = limit;
  // Intialize others
  pid->target = target;
  pid->output = 0;
}

double PID_Position_Calc(PID_TypeDef *pid, const double actual) {
  // calculate the errors
  pid->error[0] = pid->target - actual;
  pid->error_sum += pid->error[0];
  pid->error_diff = pid->error[0] - pid->error[1];

  // calculate the output
  pid->output = pid->Kp * pid->error[0] +
                abs_limit(pid->Ki * pid->error_sum, pid->integral_max) +
                pid->Kd * pid->error_diff;

  // resure that the output won't be larger than the limit
  pid->output = abs_limit(pid->output, pid->output_max);
  return pid->output;
}

double PID_Increasement_Calc(PID_TypeDef *pid, const double actual) {
  // calculate the errors
  pid->error[0] = pid->target - actual;

  // calculate the output
  pid->output = pid->Kp * (pid->error[0] - pid->error[1]) +
                pid->Ki * pid->error[0] +
                pid->Kd * ((pid->error[0] - 2 * pid->error[1] + pid->error[2]));

  // shift error
  pid->error[2] = pid->error[1];
  pid->error[1] = pid->error[0];

  // resure that the output won't be larger than the limit
  pid->output = abs_limit(pid->output, pid->output_max);
  return pid->output;
}