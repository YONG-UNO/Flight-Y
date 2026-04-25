//
// Created by DingYong on 2026/4/25.
//

#pragma once

#include "FreeRTOS.h"
#include "PID.hpp"
#include "Matrix.hpp"

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
        matrix::Vector3f actual_gyro,float dt,bool update_integral)
    {
        // run 3 PIDs
        matrix::Vector3f torque;

        _roll_pid.setSetpoint(target_rate.x);
        torque.x = _roll_pid.update(actual_gyro.x,dt,update_integral);
        _roll_pid.setSetpoint(target_rate.y);
        torque.y = _roll_pid.update(actual_gyro.y,dt,update_integral);
        _roll_pid.setSetpoint(target_rate.z);
        torque.z = _roll_pid.update(actual_gyro.z,dt,update_integral);

        return torque;
    }

private:
    PID _roll_pid;
    PID _pitch_pid;
    PID _yaw_pid;
};
