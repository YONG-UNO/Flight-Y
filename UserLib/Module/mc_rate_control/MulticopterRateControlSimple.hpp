//
// Created by DingYong on 2026/4/25.
//

#pragma once

#include "PID.hpp"

namespace matrix
{
    class Vector3f;
}

class MulticopterRateControlSimple
{
public:
    MulticopterRateControlSimple()
    {
        _roll_pid.setGains(0.15f,0.1f,0.002f);
        _pitch_pid.setGains(0.15f,0.1f,0.002f);
        _yaw_pid.setGains(0.20f,0.05f,0.0f);

        _roll_pid.setOutputLimit(1.0f);
        _pitch_pid.setOutputLimit(1.0f);
        _yaw_pid.setOutputLimit(1.0f);
    }

    // main control function
    matrix::Vector3f run(matrix::Vector3f target_rate,
        matrix::Vector3f actual_gyro,)
    {

    }
private:
    PID _roll_pid;
    PID _pitch_pid;
    PID _yaw_pid;
};
