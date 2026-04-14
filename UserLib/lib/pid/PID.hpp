//
// Created by DingYong on 2026/4/10.
//

#pragma once

#include <cmath>

class PID
{
public:
    // 默认构造
    PID() = default;
    virtual ~PID() = default;
    // 限幅
    void setOutputLimit(const float limit) {_limit_integral = limit;}
    void setIntegralLimit(const float limit){_limit_integral = limit;}
    // PID 参数 + 目标值
    void setGains(const float P, const float I, const float D);
    void setSetpoint(const float setpoint){_setpoint = setpoint;}
    // 核心计算函数
    float update(const float feedback, const float dt, const bool update_integral = true);
    // 积分/微分 重置
    float getIntegral() { return _integral; }
    void resetIntegral() { _integral = 0.0f; }
private:
    void updateIntegral(float error, const float dt);
    float updateDerivative(float feedback, const float dt);

    float _setpoint = 0.0f; // current setpoint to track
    float _integral = 0.0f; // integral state 官方叫做积分状态貌似是因为积分是累加状态
    float _last_feedback = NAN;
    // Gains,Limits
    float _gain_proportional = 0.0f;
    float _gain_integral     = 0.0f;
    float _gain_derivative   = 0.0f;
    float _limit_integral    = 0.0f;
    float _limit_output      = 0.0f;
};