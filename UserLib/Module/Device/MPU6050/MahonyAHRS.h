// MahonyAHRS.h - 纯C版，适配STM32 GCC编译
#ifndef MAHONYAHRS_H
#define MAHONYAHRS_H

#include <stdint.h>
#include <math.h>

// 宏定义（可根据需求修改）
#define DEFAULT_SAMPLE_FREQ	300.0f	// 默认采样频率300Hz
#define twoKpDef	(2.0f * 5.0f)	// 2*比例增益Kp
#define twoKiDef	(2.0f * 0.8f)	// 2*积分增益Ki
#define DEG_TO_RAD	0.0174533f		// 角度转弧度
#define RAD_TO_DEG	57.29578f		// 弧度转角度

// Mahony算法参数结构体（替代C++的class，封装所有私有变量）
typedef struct {
    float twoKp;		// 2*Kp
    float twoKi;		// 2*Ki
    float q0, q1, q2, q3;// 四元数
    float integralFBx, integralFBy, integralFBz; // 积分误差
    float invSampleFreq;// 1/采样频率
    float roll, pitch, yaw; // 欧拉角
    uint8_t anglesComputed; // 角度是否已计算标志
} Mahony_t;

// 全局Mahony算法实例（替代原来的全局class对象，直接调用）
extern Mahony_t mahony;

// 函数声明（纯C语法，无成员函数）
void Mahony_Init(void); // 初始化（替代C++的构造函数）
void Mahony_Begin(float sampleFrequency); // 设置采样频率
void Mahony_UpdateIMU(float gx, float gy, float gz, float ax, float ay, float az); // 仅IMU融合
float Mahony_GetRoll(void); // 获取横滚角（度）
float Mahony_GetPitch(void); // 获取俯仰角（度）
float Mahony_GetYaw(void); // 获取偏航角（度）
float Mahony_GetRollRadians(void); // 获取横滚角（弧度）
float Mahony_GetPitchRadians(void); // 获取俯仰角（弧度）
float Mahony_GetYawRadians(void); // 获取偏航角（弧度）

#endif