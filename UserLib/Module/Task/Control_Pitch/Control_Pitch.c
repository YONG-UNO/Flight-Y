//
// Created by DingYong on 2026/2/10.
//
#include "Control_Pitch.h"

#include "cmsis_os2.h"
#include "pid.h"
#include "integration.h"
#include "tim.h"
#include "infrared_ranging.h"
#include "optical_flow.h"

#define PWM_BASE 800



static pid_t roll_position_pid = {
    .kp = 5.0f,  //5
    .ki = 0.1f,  // 0.1
    .kd = 0.0f,
    .i_out_limit = 100,
    .sum_out_limit = 300
};


static pid_t pitch_position_pid = {
    .kp = 5.0f,
    .ki = 0.1f,
    .kd = 0.0f,
    .i_out_limit = 100,
    .sum_out_limit = 300
};

static pid_t yaw_position_pid = {
    .kp = 0.0f,
    .ki = -0.1f,
    .kd = 0.0f,
    .i_out_limit = 0,
    .sum_out_limit = 0
};


// hight
static pid_t hight_speed_pid = {
    .kp = 5.0f,
    .ki = 0.0f,
    .kd = 0.0f,
    .i_out_limit = 0,
    .sum_out_limit = 200
};

static pid_t hight_position_pid = {
    .kp = 1.0f,
    .ki = 0.001f,
    .kd = 5.0f,
    .i_out_limit = 50,
    .sum_out_limit = 120
};

// x
static pid_t x_speed_pid = {
    .kp = 0.2f,
    .ki = 0.0f,
    .kd = 0.0f,
    .i_out_limit = 0,
    .sum_out_limit = 0
};

// y
static pid_t y_speed_pid = {
    .kp = 0.2f,
    .ki = 0.0f,
    .kd = 0.0f,
    .i_out_limit = 0,
    .sum_out_limit = 0
};

int64_t time_count = 0;

static int16_t roll_position_out, roll_speed_out, filtered_roll_gyro;
static int16_t pitch_position_out, pitch_speed_out,filtered_pitch_gyro;
static int16_t yaw_speed_out,filtered_yaw_gyro;


static int16_t hight_speed_out, hight_position_out, speed, last_distance = 0;
static int16_t x_speed_out,y_speed_out;

static int16_t pwm[4] = {0};

void Control_Pitch(void *argument) {
    (void)argument;


    osDelay(4000);

    for (;;) {

        // roll
        filtered_roll_gyro = 0.2 * MPU6050.gyro[0] + 0.8 * filtered_roll_gyro;
        roll_speed_out = -roll_position_pid.kp * (0-MPU_Attitude_Angle[0]) + roll_position_pid.ki * filtered_roll_gyro;


        // pitch
        filtered_pitch_gyro = 0.2 * MPU6050.gyro[1] + 0.8 * filtered_pitch_gyro;
        pitch_speed_out = pitch_position_pid.kp * (0-MPU_Attitude_Angle[1]) - pitch_position_pid.ki * filtered_pitch_gyro;

        // yaw
        filtered_yaw_gyro = 0.2 * MPU6050.gyro[2] + 0.8 * filtered_yaw_gyro;
        yaw_speed_out = yaw_position_pid.kp * (60-MPU_Attitude_Angle[2]) - yaw_position_pid.ki * filtered_yaw_gyro;

        // hight
        if (ranging_data.distance >= 200) ranging_data.distance = 100;
        static int16_t filitered_distance;
        filitered_distance = 0.2 * ranging_data.distance + 0.8 * filitered_distance;
        pid_calc(&hight_position_pid, 100, filitered_distance);
        hight_position_out = hight_position_pid.sum_out;

        speed = (ranging_data.distance - last_distance)*100;
        pid_calc(&hight_speed_pid, hight_position_out, speed);
        hight_speed_out = hight_speed_pid.sum_out;
        last_distance = ranging_data.distance;

        // x/y
        x_speed_out = 0;// -x_speed_pid.kp * (0-flow.flow_x_i) + x_speed_pid.ki * flow.flow_x;
        y_speed_out = 0;//-y_speed_pid.kp * (0-flow.flow_y_i) + y_speed_pid.ki * flow.flow_y;


        pwm[0] = PWM_BASE + roll_speed_out + pitch_speed_out + hight_speed_out + x_speed_out + y_speed_out + yaw_speed_out;  // 左前电机
        pwm[1] = PWM_BASE - roll_speed_out + pitch_speed_out + hight_speed_out - x_speed_out + y_speed_out - yaw_speed_out;  // 右前电机
        pwm[2] = PWM_BASE - roll_speed_out - pitch_speed_out + hight_speed_out - x_speed_out - y_speed_out + yaw_speed_out;  // 右后电机
        pwm[3] = PWM_BASE + roll_speed_out - pitch_speed_out + hight_speed_out + x_speed_out - y_speed_out - yaw_speed_out;  // 左后电机


        // 设置PWM值
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1, pwm[1]);  // motor2右前
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, pwm[0]);  // motor1左前
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3, pwm[2]);  // motor3右后
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4, pwm[3]);  // motor4左后

        osDelay(1);

        // if (time_count <= 10000) {
        //     time_count++;
        // }else {
        //     while (1) {
        //         __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1, 0);  // motor2右前
        //         __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 0);  // motor1左前
        //         __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3, 0);  // motor3右后
        //         __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4, 0);  // motor4左后
        //         osDelay(1);
        //     }
        // }
    }
}