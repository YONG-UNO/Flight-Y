// MahonyAHRS.c - 纯C版，适配STM32 GCC编译
#include "MahonyAHRS.h"
#include <math.h>
#include <stdint.h>

// 定义全局Mahony实例（所有函数操作该实例，替代class的this指针）
Mahony_t mahony;

// 快速逆平方根（原算法保留，提升效率）
static float invSqrt(float x) {
	float halfx = 0.5f * x;
	union { float f; int32_t l; } i;
	i.f = x;
	i.l = 0x5f3759df - (i.l >> 1);
	float y = i.f;
	y = y * (1.5f - (halfx * y * y));
	y = y * (1.5f - (halfx * y * y));
	return y;
}

// 四元数转欧拉角（内部函数，替代class私有函数）
static void computeAngles(Mahony_t *inst) {
	inst->roll = atan2f(inst->q0*inst->q1 + inst->q2*inst->q3, 0.5f - inst->q1*inst->q1 - inst->q2*inst->q2);
	inst->pitch = asinf(-2.0f * (inst->q1*inst->q3 - inst->q0*inst->q2));
	inst->yaw = atan2f(inst->q1*inst->q2 + inst->q0*inst->q3, 0.5f - inst->q2*inst->q2 - inst->q3*inst->q3);
	inst->anglesComputed = 1;
}

// 初始化（替代C++构造函数）
void Mahony_Init(void) {
	mahony.twoKp = twoKpDef;
	mahony.twoKi = twoKiDef;
	mahony.q0 = 1.0f;
	mahony.q1 = 0.0f;
	mahony.q2 = 0.0f;
	mahony.q3 = 0.0f;
	mahony.integralFBx = 0.0f;
	mahony.integralFBy = 0.0f;
	mahony.integralFBz = 0.0f;
	mahony.anglesComputed = 0;
	mahony.invSampleFreq = 1.0f / DEFAULT_SAMPLE_FREQ;
}

// 设置采样频率
void Mahony_Begin(float sampleFrequency) {
	mahony.invSampleFreq = 1.0f / sampleFrequency;
}

// 仅IMU融合（加速度计+陀螺仪，无磁力计）
void Mahony_UpdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// 陀螺仪角度转弧度
	gx *= DEG_TO_RAD;
	gy *= DEG_TO_RAD;
	gz *= DEG_TO_RAD;

	// 加速度计数据有效时才计算反馈（避免除0）
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
		// 归一化加速度计数据
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// 估计重力方向
		halfvx = mahony.q1 * mahony.q3 - mahony.q0 * mahony.q2;
		halfvy = mahony.q0 * mahony.q1 + mahony.q2 * mahony.q3;
		halfvz = mahony.q0 * mahony.q0 - 0.5f + mahony.q3 * mahony.q3;

		// 计算误差（重力估计值与测量值的叉乘）
		halfex = (ay * halfvz - az * halfvy);
		halfez = (ax * halfvy - ay * halfvx);
		halfey = (az * halfvx - ax * halfvz);

		// 积分反馈（有Ki时）
		if(mahony.twoKi > 0.0f) {
			mahony.integralFBx += mahony.twoKi * halfex * mahony.invSampleFreq;
			mahony.integralFBy += mahony.twoKi * halfey * mahony.invSampleFreq;
			mahony.integralFBz += mahony.twoKi * halfez * mahony.invSampleFreq;
			// 应用积分补偿
			gx += mahony.integralFBx;
			gy += mahony.integralFBy;
			gz += mahony.integralFBz;
		} else {
			// 无Ki时清空积分，防止饱和
			mahony.integralFBx = 0.0f;
			mahony.integralFBy = 0.0f;
			mahony.integralFBz = 0.0f;
		}

		// 比例反馈
		gx += mahony.twoKp * halfex;
		gy += mahony.twoKp * halfey;
		gz += mahony.twoKp * halfez;
	}

	// 积分更新四元数
	gx *= (0.5f * mahony.invSampleFreq);
	gy *= (0.5f * mahony.invSampleFreq);
	gz *= (0.5f * mahony.invSampleFreq);
	qa = mahony.q0;
	qb = mahony.q1;
	qc = mahony.q2;
	mahony.q0 += (-qb * gx - qc * gy - mahony.q3 * gz);
	mahony.q1 += (qa * gx + qc * gz - mahony.q3 * gy);
	mahony.q2 += (qa * gy - qb * gz + mahony.q3 * gx);
	mahony.q3 += (qa * gz + qb * gy - qc * gx);

	// 归一化四元数（保证模长为1）
	recipNorm = invSqrt(mahony.q0 * mahony.q0 + mahony.q1 * mahony.q1 + mahony.q2 * mahony.q2 + mahony.q3 * mahony.q3);
	mahony.q0 *= recipNorm;
	mahony.q1 *= recipNorm;
	mahony.q2 *= recipNorm;
	mahony.q3 *= recipNorm;
	mahony.anglesComputed = 0;
}

// 获取横滚角（度）
float Mahony_GetRoll(void) {
	if (!mahony.anglesComputed) computeAngles(&mahony);
	return mahony.roll * RAD_TO_DEG;
}

// 获取俯仰角（度）
float Mahony_GetPitch(void) {
	if (!mahony.anglesComputed) computeAngles(&mahony);
	return mahony.pitch * RAD_TO_DEG;
}

// 获取偏航角（度，0~360°）
float Mahony_GetYaw(void) {
	if (!mahony.anglesComputed) computeAngles(&mahony);
	return mahony.yaw * RAD_TO_DEG + 180.0f;
}

// 获取横滚角（弧度）
float Mahony_GetRollRadians(void) {
	if (!mahony.anglesComputed) computeAngles(&mahony);
	return mahony.roll;
}

// 获取俯仰角（弧度）
float Mahony_GetPitchRadians(void) {
	if (!mahony.anglesComputed) computeAngles(&mahony);
	return mahony.pitch;
}

// 获取偏航角（弧度）
float Mahony_GetYawRadians(void) {
	if (!mahony.anglesComputed) computeAngles(&mahony);
	return mahony.yaw;
}