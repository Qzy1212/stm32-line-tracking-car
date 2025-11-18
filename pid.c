#include "pid.h"

void PID_Init(PID_Controller_t *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint   = 0.0f;
    pid->error      = 0.0f;
    pid->last_error = 0.0f;
    pid->integral   = 0.0f;
    pid->output     = 0.0f;
    pid->out_max    = 500.0f;
    pid->out_min    = -500.0f;
}

float PID_Calculate(PID_Controller_t *pid, float measured)
{
    pid->error = pid->setpoint - measured;
    pid->integral += pid->error;
    pid->output = pid->kp * pid->error
                + pid->ki * pid->integral
                + pid->kd * (pid->error - pid->last_error);
    pid->last_error = pid->error;

    if (pid->output > pid->out_max) pid->output = pid->out_max;
    if (pid->output < pid->out_min) pid->output = pid->out_min;
    return pid->output;
}

/* 抗积分饱和: 当输出已达限幅时停止积分累加 */
float PID_Calculate_AntiWindup(PID_Controller_t *pid, float measured)
{
    pid->error = pid->setpoint - measured;

    /* 仅在未饱和时累加积分 */
    if (pid->output < pid->out_max && pid->output > pid->out_min) {
        pid->integral += pid->error;
    }

    pid->output = pid->kp * pid->error
                + pid->ki * pid->integral
                + pid->kd * (pid->error - pid->last_error);
    pid->last_error = pid->error;

    if (pid->output > pid->out_max) pid->output = pid->out_max;
    if (pid->output < pid->out_min) pid->output = pid->out_min;
    return pid->output;
}
