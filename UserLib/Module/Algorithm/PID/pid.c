//
// Created by DingYong on 25-8-8.
//

#include "pid.h"

void pid_calc(pid_t *pid, float target, float feedback) {

    pid->target = target;
    pid->feedback = feedback;

    pid->error[1] = pid->error[0];
    pid->error[0] = target - feedback;

    pid->p_out = pid->kp * pid->error[0];    // p_out
    pid->i_out += pid->ki * pid->error[0];   // i_out
    pid->d_out = pid->kd * (pid->error[0] - pid->error[1]);

    if (pid->i_out > pid->i_out_limit) pid->i_out = pid->i_out_limit;
    if (pid->i_out < -pid->i_out_limit) pid->i_out = -pid->i_out_limit;

    pid->sum_out = pid->p_out + pid->i_out + pid->d_out;

    if (pid->sum_out > pid->sum_out_limit) pid->sum_out = pid->sum_out_limit;
    if (pid->sum_out < -pid->sum_out_limit) pid->sum_out = -pid->sum_out_limit;

}



void pitch_pid_init(pid_t *angularVelocity_pid,pid_t *angular_pid){
    // Id环(将d轴电流压到0,避免电机磁链波动)
    angularVelocity_pid->kp = 0.0f;
    angularVelocity_pid->ki = 0.0f;
    angularVelocity_pid->kd = 0.0f;
    angularVelocity_pid->i_out_limit = 0.0f;
    angularVelocity_pid->sum_out_limit = 0.0f;

    // Iq(力矩/电流环)
    // 不写kd减小对高频噪声的进一步放大
    angular_pid->kp = 0.0f;
    angular_pid->ki = 0.0f;
    angular_pid->kd = 0.0f;
    angular_pid->i_out_limit = 0.0f;
    angular_pid->sum_out_limit = 0.0f;
}