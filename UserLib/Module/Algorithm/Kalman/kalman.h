#ifndef KALMAN_H
#define KALMAN_H

#include <stdint.h>

// 卡尔曼滤波结构体
typedef struct _1_ekf_filter {
    float Q;      // 过程噪声协方差
    float R;      // 测量噪声协方差
    float P;      // 估计误差协方差
    float x_hat;  // 状态估计值
    float K;      // 卡尔曼增益
    float out;    // 滤波输出值
} KalmanFilter;

// 卡尔曼滤波函数
void kalman_1(KalmanFilter *ekf, float input);

#endif // KALMAN_H