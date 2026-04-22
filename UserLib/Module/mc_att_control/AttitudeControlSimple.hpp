//
// Created by DingYong on 2026/4/22.
//

#pragma once
#include "Matrix.hpp"

extern "C" {

class AttitudeControlSimple
{
public:
    AttitudeControlSimple() = default;

    // set attitude term p_term gain
    void setProportionalGain(const matrix::Vector3f &proportioanl_gain);
    // input:current quatf, output:target angle speed
    matrix::Vector3f update(const matrix::Quatf &q) const;

    // set target attitude quatf
    void setAttitudeSetpoint(const matrix::Quatf &q)
    {
        _attitude_setpoint_q = q;
    }
private:
    matrix::Quatf _attitude_setpoint_q; // target_attitude
    matrix::Vector3f _proportional_gain; // attitude p_term_gain
};

}

