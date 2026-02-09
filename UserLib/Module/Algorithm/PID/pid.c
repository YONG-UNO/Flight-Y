//
// Created by DingYong on 25-8-8.
//

#include "pid.h"

void pid_calc(pid_t *pid, float target, float feedback) {

    pid->target = target;
    pid->feedback = feedback;

    pid->error[1] = pid->error[0];
    pid->error[0] = feedback - target;

    pid->p_out = pid->kp * pid->error[0];    // p_out
    pid->i_out += pid->ki * pid->error[0];   // i_out
    pid->d_out = pid->kd * (pid->error[0] - pid->error[1]);

    if (pid->i_out > pid->i_out_limit) pid->i_out = pid->i_out_limit;
    if (pid->i_out < -pid->i_out_limit) pid->i_out = -pid->i_out_limit;

    pid->sum_out = pid->p_out + pid->i_out + pid->d_out;

    if (pid->sum_out > pid->sum_out_limit) pid->sum_out = pid->sum_out_limit;
    if (pid->sum_out < -pid->sum_out_limit) pid->sum_out = -pid->sum_out_limit;

}