//
// Created by azarai on 7/24/24.
//

#ifndef PID_H
#define PID_H

typedef long unsigned int uint32_t;

typedef struct PID_TypeDef {
  double Kp;  //  Proportional Const
  double Ki;  //  Integral Const
  double Kd;  //  Derivative Const

  double error[3];    // Error [0, -1, -2]
  double error_diff;  // Difference of Error[0] - Error[-1]
  double error_sum;   // Sum of the error

  uint32_t output;
  uint32_t target;

  double integral_max;  // Max of Integral
  double output_max;    // Max of Output
} PID_TypeDef;

double abs_limit(double value, double max);
void PID_Init(PID_TypeDef *pid, double Kp, double Ki, double Kd, double limit,
              const uint32_t target);
double PID_Position_Calc(PID_TypeDef *pid, double actual);
double PID_Increasement_Calc(PID_TypeDef *pid, double actual);

#endif  // PID_H
