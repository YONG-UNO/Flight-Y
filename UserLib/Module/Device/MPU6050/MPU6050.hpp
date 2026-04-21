/***********************************************************************
 * Project Name    : Flight-Y STM32 Flight Controller
 * MCU Platform    : STM32F103C8T6 (Cortex-M3)
 * Author          : Ding Yong
 * Create Time     : 2026-04-17
 *
 * Version Info:
 *  - V1.0.0  2026-04-17  Initial version
 *                          > MPU6050 driver & basic data read
 *                          > Kalman filter & LPF for accelerometer/gyro
 *                          > PID C++ class integration
 *
 *  - V1.0.0  YYYY-MM-DD  Description
 *
 * Brief: Main flight controller with attitude calculation and PID control
 ***********************************************************************/

/***********************************************************************
 * Project Name    : Flight-Y STM32 Flight Controller
 * MCU Platform    : STM32F103C8T6 (Cortex-M3)
 * Author          : Ding Yong
 * Create Time     : 2026-04-17
 *
 * Version Info:
 *  - V1.0.0  2026-04-17  Initial version
 *                          > MPU6050 driver & basic data read
 *                          > Kalman filter & LPF for accelerometer/gyro
 *                          > PID C++ class integration
 *
 *  - V1.0.0  2026-4-17
 *
 * Brief: Main flight controller with attitude calculation and PID control
 ***********************************************************************/
#ifndef FLIGHT_Y_MPU6050_HPP
#define FLIGHT_Y_MPU6050_HPP

#include "stm32f1xx_hal.h"
#include "StatusTypeDef_Y.hpp"


#define MPU6050_DEBUG 1
// 宏定义,参考手册

// 弧度/角度转换宏
#define DEG_TO_RAD 0.017453292519943295f
#define RAD_TO_DEG 57.29577951308232f

// MPU6050核心定义（贴合手册寄存器规范）
#define MPU6050_ADDR_7BIT  0x68    // 7位I2C地址（AD0接地）
#define MPU6050_WHO_AM_I_VAL 0x68 // WHO_AM_I默认值（手册4.32节）
#define MPU6050_REG_DATA_START 0x3B // 数据寄存器起始地址（0x3B~0x48，共14字节）
#define MPU6050_DATA_LEN 14       // 单次读取字节数（加速度6+温度2+陀螺仪6）

// 寄存器地址（手册3节寄存器映射）
#define REG_SMPLRT_DIV   0x19   // 采样率分频
#define REG_CONFIG       0x1A   // DLPF配置
#define REG_GYRO_CONFIG  0x1B   // 陀螺仪量程
#define REG_ACCEL_CONFIG 0x1C   // 加速度计量程
#define REG_PWR_MGMT_1   0x6B   // 电源管理
#define REG_WHO_AM_I     0x75   // 设备ID

// 量程灵敏度（手册6.1/6.2节）
#define ACC_FS_4G_LSB  8192.0f   // ±4G → 8192 LSB/g
#define GYRO_FS_2000DPS_LSB 16.4f// ±2000°/s → 16.4 LSB/(°/s)

// 数据结构体
typedef struct {
    // 原始数据
    int16_t acc_raw[3];   // 加速度 X/Y/Z (LSB)
    int16_t gyro_raw[3];  // 陀螺仪 X/Y/Z (LSB)
    int16_t temp_raw;     // 温度原始值 (LSB)
    // 转换后物理量
    float acc[3];         // 加速度 (g)
    float gyro[3];        // 陀螺仪 (°/s)
    float temperature;     // 温度 (℃)
} MPU6050_Data_t;

class MPU6050
{
public:
    MPU6050(I2C_HandleTypeDef *hi2c)
        : _hi2c(hi2c){}
    MPU6050_Data_t data;

    StatusTypeDef_Y init(void);
    void readData(void);

private:

    I2C_HandleTypeDef *_hi2c;

    StatusTypeDef_Y I2C_Write(uint8_t reg, uint8_t data); // IIC写1字节
    StatusTypeDef_Y I2C_Read(uint8_t reg, uint8_t *data, uint16_t len); // IIC读多字节
    void i2cBusReset();

};

#endif //FLIGHT_Y_MPU6050_HPP