/**
* @file PID.hpp
 * @brief 通用PID控制器
 * @author DingYong
 * @version V1.0.0
 * @date 2026-04-27
 *
 * @history
 * 版本      日期        作者       变更内容
 * V1.0.0    2026-04-27  DingYong   初版创建
 * V1.0.1    2026-04-28  DingYong   优化积分限幅,增加死区
 */

#include "PID.hpp"
#include "Functions.hpp"
#include <float.h>

void PID::setGains(const float P, const float I, const float D) {
    _gain_proportional = P;
    _gain_integral = I;
    _gain_derivative = D;
}

float PID::update(const float feedback, const float dt, const bool update_integral) {
    const float error = _setpoint - feedback;
    const float output = (_gain_proportional * error) + _integral + (_gain_derivative * updateDerivative(feedback,dt));

    if (update_integral) {                // 积分分离:避免解锁前积分无限大
        updateIntegral(error,dt);
    }

    _last_feedback = feedback;
    return math::constrain(output, -_limit_output, _limit_output);
}

// @todo:add cconstrain
void PID::updateIntegral(float error, const float dt) {
    const float integral_new = _integral + _gain_integral * error * dt;
}

float PID::updateDerivative(float feedback, const float dt) {
    float feedback_change = 0.0f;

    if ((dt > FLT_EPSILON) && std::isfinite(_last_feedback)) { // FLT_EPSILON:float 类型能表示的最小精度值,小数点后六位,也用来判断float ==
        feedback_change = (feedback - _last_feedback) / dt;
    }

    return feedback_change;
}