//
// Created by DingYong on 2026/4/23.
//

#include "attitude_loop_task.hpp"

#include "cmsis_os.h"
#include "AttitudeControlSimple.hpp"
#include "IMUupdata.hpp"

// global veriable
AttitudeControlSimple att_controller_test;

matrix::Vector3f desired_speed_rate;


void attitude_loop_task(void const * argument)
{
    // set the attitude p_term gain
    att_controller_test.setProportionalGain(matrix::Vector3f(20.0f,20.0f,3.0f));
    // set the desired set point
    att_controller_test.setAttitudeSetpoint(matrix::Quatf(1,0,0,0));

    for (;;)
    {
        // extract the current quaternary number
        matrix::Quatf current_attitude = mahony.getQuaternion();
        desired_speed_rate = att_controller_test.update(current_attitude);

        osDelay(1);
    }
}
