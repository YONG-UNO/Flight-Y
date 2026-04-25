//
// Created by DingYong on 2026/4/22.
//

#include "AttitudeControlSimple.hpp"

void AttitudeControlSimple::setProportionalGain(const matrix::Vector3f &proportioanl_gain)
{
    _proportional_gain = proportioanl_gain;
}

matrix::Vector3f AttitudeControlSimple::update(const matrix::Quatf& q) const
{
    // desired attitude
    matrix::Quatf qd = _attitude_setpoint_q;

    // attitude error = current attitude.inversed() * desired attitude
    matrix::Quatf q_error = q.inversed() * qd;

    // imag(): extract the error vector(x, y, z)
    // x = u_x * sin(sita / 2)
    // x = u_x * (sita/2)
    // u_x * sita = x * 2.0f, the u_x * sita is the real physical angle error
    matrix::Vector3f error = q_error.imag() * 2.0f;

    // attitude p_term calculate -> desired angle speed
    matrix::Vector3f rate_setpoint = error.emult(_proportional_gain);

    return rate_setpoint;
}
