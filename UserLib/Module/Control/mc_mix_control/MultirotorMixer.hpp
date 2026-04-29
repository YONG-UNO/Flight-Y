/**
* @file MultirotorMixer.hpp
 * @brief 多旋翼混控器（四轴X型）
 * @author DingYong
 * @version V1.0.0
 * @date 2026-04-27
 *
 * @history
 * 版本      日期        作者       变更内容
 * V1.0.0    2026-04-27  DingYong   初版创建,支持四轴X混控
 */
#pragma once

#include "mathlib.hpp"
#include "Matrix.hpp"

// 最多支持的电机数
#define MAX_MOTORS 8

class MultirotorMixer
{
public:

    // 四轴 X 型混控计算
    // torque: 来自 PID 的 [roll, pitch, yaw] 力矩
    // throttle: 总油门 0~1
    // motor_out: 输出 4 个电机油门
    void mixQuadX(matrix::Vector3f torque,float throttle,float motor_out[MAX_MOTORS])
    {
        // 四轴 X 型混控矩阵（正逻辑：力矩越大 -> 对应电机差越大）
        // 电机顺序：0=前左  1=前右  2=后右  3=后左
        motor_out[0] = throttle + torque.x - torque.y + torque.z;
        motor_out[1] = throttle - torque.x - torque.y - torque.z;
        motor_out[2] = throttle - torque.x + torque.y + torque.z;
        motor_out[3] = throttle + torque.x + torque.y - torque.z;

        // 归一化,不为负数,不超油门
        motor_out[0] = math::constrain(motor_out[0],0.0f,1.0f);
        motor_out[1] = math::constrain(motor_out[1],0.0f,1.0f);
        motor_out[2] = math::constrain(motor_out[2],0.0f,1.0f);
        motor_out[3] = math::constrain(motor_out[3],0.0f,1.0f);
    }
};
