#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct {
    float kp;
    float ki;
    float kd;
    float setpoint;
    float error;
    float last_error;
    float integral;
    float output;
    float out_max;
    float out_min;
} PID_Controller_t;

void  PID_Init(PID_Controller_t *pid, float kp, float ki, float kd);
float PID_Calculate(PID_Controller_t *pid, float measured);

#endif /* __PID_H */
