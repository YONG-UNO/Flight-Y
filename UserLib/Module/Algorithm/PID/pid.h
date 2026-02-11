//
// Created by DingYong on 25-8-8.
//

#ifndef PID_H
#define PID_H

typedef struct {
    float kp;
    float ki;
    float kd;

    float p_out;
    float i_out;
    float d_out;
    float sum_out;

    float i_out_limit;
    float sum_out_limit;

    float target;
    float feedback;
    float error[2];
} pid_t;

void pid_calc(pid_t *pid, float target, float feedback);
void pitch_pid_init(pid_t *angularVelocity_pid,pid_t *angular_pid);

#endif //PID_H