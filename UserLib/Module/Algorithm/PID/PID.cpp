//
// Created by DingYong on 2026/4/10.
// Version V1.0.1
// Brief 修改于原C语言版本
//

#include "PID.hpp"

void PID::setGains(const float P, const float I, const float D) {
    _gain_proportional = P;
    _gain_integral = I;
    _gain_derivative = D;
}

// @todo:add cconstrain
void PID::updateIntegral(float error, const float dt) {
    const float integral_new = _integral + _gain_integral * error * dt;
}

void PID::updateDerivative(float feedback, const float dt) {
    float feedback_change = 0.0f;

    if ((dt > FLT_EPSILON) && std::isfinite(_last_feedback)) {
        feedback_change = (feedback - _last_feedback) / dt;
    }

    return feedback_change;
}