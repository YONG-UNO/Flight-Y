// MPU_Integration.h
#ifndef MPU_INTEGRATION_H
#define MPU_INTEGRATION_H

#include "MPU6050.h"

// 原有全局变量声明（保留）
extern float Gyro_Offset[3];  // 陀螺仪偏移（工程已声明，直接复用）

// 新增：全局姿态角数组 [0]=roll 横滚, [1]=pitch 俯仰, [2]=yaw 偏航（单位：度）
extern float MPU_Attitude_Angle[3];
// 新增：校准完成标志 0=未校准 1=已校准（判断是否完成3秒静止校准）
extern uint8_t MPU_Calibration_Done;

// 可选：添加姿态解算任务声明（如果在该文件声明任务）
void Integration(void const * argument);

#endif