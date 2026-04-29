/**
* @file mathlib
 * @brief 通用math库
 * @author DingYong
 * @version V1.0.0
 * @date 2026-04-14
 *
 * @history
 * 版本      日期        作者       变更内容
 * V1.0.0    2026-04-14  DingYong   初版创建
 * V1.0.1    2026-04-28  DingYong   从Functions改名为mathlib
 * V1.1.0    2026.4.29   DingYong   封装一阶卡尔曼滤波到 mathlib
 */

#pragma once



class math
{
public:
    static float constrain(float val ,float min_val,float max_val)
    {
        return (val < min_val) ? min_val : ((val > max_val) ? max_val : val);
    }

    class kalman
    {
    public:
        // 一阶卡尔曼滤波结构体
        typedef struct {
            float Q;      // 过程噪声
            float R;      // 观测噪声
            float P;      // 估计协方差
            float x_hat;  // 最优估计
            float K;      // 卡尔曼增益
            float out;    // 输出
        } KalmanFilter;

    public:
        // 卡尔曼结构体
        typedef struct {
            float Q;
            float R;
            float P;
            float x_hat;
            float K;
            float out;
        } Filter;

        // 初始化
        static void init(Filter *kf, float Q, float R)
        {
            kf->Q = Q;
            kf->R = R;
            kf->P = 1.0f;
            kf->x_hat = 0.0f;
            kf->K = 0.0f;
            kf->out = 0.0f;
        }

        // 更新滤波
        static float update(Filter *kf, float input)
        {
            // 预测
            kf->P = kf->P + kf->Q;

            // 卡尔曼增益
            kf->K = kf->P / (kf->P + kf->R);

            // 校正
            kf->x_hat = kf->x_hat + kf->K * (input - kf->x_hat);

            // 更新误差
            kf->P = (1 - kf->K) * kf->P;

            kf->out = kf->x_hat;
            return kf->out;
        }
    };
};
