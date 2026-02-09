#include "kalman.h"

/**
  * @brief  一维卡尔曼滤波
  * @param  ekf: 滤波结构体
  * @param  input: 原始输入值
  */
void kalman_1(KalmanFilter *ekf, float input) {
    // 预测
    ekf->P = ekf->P + ekf->Q;

    // 更新卡尔曼增益
    ekf->K = ekf->P / (ekf->P + ekf->R);

    // 修正估计值
    ekf->x_hat = ekf->x_hat + ekf->K * (input - ekf->x_hat);

    // 更新协方差
    ekf->P = (1 - ekf->K) * ekf->P;

    // 输出滤波后的值
    ekf->out = ekf->x_hat;
}