#include "MahonyAHRS.h"
#include "MPU6050.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <math.h>

// 全局变量：外部可直接访问最终姿态角
float MPU_Attitude_Angle[3] = {0.0f, 0.0f, 0.0f}; // 最终角度（已减误差）
uint8_t MPU_Calibration_Done = 0;                 // 误差计算完成标志

// 核心配置：仅需改这两个参数
#define SAMPLE_FREQ 300                // 采样频率（300Hz，每3ms一次）
#define CALIB_DURATION 2000            // 误差计算时长（2000ms=2秒）
#define CALIB_COUNT (CALIB_DURATION / (1000/SAMPLE_FREQ)) // 2秒采样次数（600次）

// 局部变量：仅误差计算阶段使用
static float angle_error[3] = {0.0f, 0.0f, 0.0f}; // 2秒角度平均值（固定误差）
static float gyro_offset[3] = {0.0f, 0.0f, 0.0f}; // 2秒陀螺仪零漂（辅助校准）
static uint16_t calib_cnt = 0;                     // 误差计算阶段的采样计数

// 核心任务：启动即算2秒误差，后续全程减误差
void Integration(void const * argument) {
    (void)argument;

    // 1. 初始化MPU6050和融合算法（失败则死循环）
    if(MPU6050_Init() != 0) { for(;;) { osDelay(100); } }
    Mahony_Init();
    Mahony_Begin(SAMPLE_FREQ);

    // 主循环：全程执行
    for (;;) {
        // 2. 读取MPU原始数据并转换为物理值
        MPU6050_ReadData();
        float acc_x = (float)MPU6050.acc_raw[0] / 16384.0f; // ±2g量程
        float acc_y = (float)MPU6050.acc_raw[1] / 16384.0f;
        float acc_z = (float)MPU6050.acc_raw[2] / 16384.0f;
        float gyro_x = (float)MPU6050.gyro_raw[0] / 131.0f;  // ±250°/s量程
        float gyro_y = (float)MPU6050.gyro_raw[1] / 131.0f;
        float gyro_z = (float)MPU6050.gyro_raw[2] / 131.0f;

        // ===================== 阶段1：启动后先算2秒误差（无任何判断） =====================
        if (!MPU_Calibration_Done) {
            // 累加前600次（2秒）的陀螺仪和角度数据
            if (calib_cnt < CALIB_COUNT) {
                // 融合当前角度
                Mahony_UpdateIMU(gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z);
                // 累加数据用于计算平均值
                gyro_offset[0] += gyro_x;
                gyro_offset[1] += gyro_y;
                gyro_offset[2] += gyro_z;
                angle_error[0] += Mahony_GetRoll();
                angle_error[1] += Mahony_GetPitch();
                angle_error[2] += Mahony_GetYaw();
                calib_cnt++;

                // 实时输出未减误差的原始角度（可选）
                MPU_Attitude_Angle[0] = Mahony_GetRoll();
                MPU_Attitude_Angle[1] = Mahony_GetPitch();
                MPU_Attitude_Angle[2] = Mahony_GetYaw();
            }
            // 2秒数据采集完成：计算平均值作为固定误差
            else {
                gyro_offset[0] /= CALIB_COUNT; // 陀螺仪零漂平均值
                gyro_offset[1] /= CALIB_COUNT;
                gyro_offset[2] /= CALIB_COUNT;
                angle_error[0] /= CALIB_COUNT;  // 角度误差平均值（核心）
                angle_error[1] /= CALIB_COUNT;
                angle_error[2] /= CALIB_COUNT;
                MPU_Calibration_Done = 1;       // 标记误差计算完成
            }
        }

        // ===================== 阶段2：误差计算完成后，全程减误差 =====================
        else {
            // 第一步：陀螺仪减去零漂（提升精度）
            float gx = gyro_x - gyro_offset[0];
            float gy = gyro_y - gyro_offset[1];
            float gz = gyro_z - gyro_offset[2];

            // 第二步：融合得到原始角度
            Mahony_UpdateIMU(gx, gy, gz, acc_x, acc_y, acc_z);
            float roll = Mahony_GetRoll();
            float pitch = Mahony_GetPitch();
            float yaw = Mahony_GetYaw();

            // 第三步：核心操作——直接减去前面2秒算出的固定误差
            MPU_Attitude_Angle[0] = roll - angle_error[0];
            MPU_Attitude_Angle[1] = pitch - angle_error[1];
            MPU_Attitude_Angle[2] = yaw - angle_error[2];
        }

        // 严格匹配采样频率的延时（3ms）
        osDelay(1000/SAMPLE_FREQ);
    }
}