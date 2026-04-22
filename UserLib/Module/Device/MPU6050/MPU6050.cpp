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
 *  - V2.0.0  2026-4-17  修改为cpp类模式
 *
 * Brief: Main flight controller with attitude calculation and PID control
 ***********************************************************************/

#include "MPU6050.hpp"

/**
  * @brief  IIC写1字节（底层封装）
  * @retval 0=成功 -1=失败
  */
StatusTypeDef_Y MPU6050::I2C_Write(uint8_t reg, uint8_t data)
{
    if (HAL_I2C_Mem_Write(_hi2c,MPU6050_ADDR_7BIT<<1,reg,I2C_MEMADD_SIZE_8BIT,&data,1,100) == HAL_OK)
    {
        return Y_OK;
    }
    MPU6050::i2cBusReset();
    return Y_ERROR;
}

/**
 * @brief  IIC读多字节（底层封装）
 * @retval 0=成功 -1=失败
 */
StatusTypeDef_Y MPU6050::I2C_Read(uint8_t reg, uint8_t* data, uint16_t len)
{
    if (HAL_I2C_Mem_Read(_hi2c,MPU6050_ADDR_7BIT<<1,reg,I2C_MEMADD_SIZE_8BIT,data,len,100) == HAL_OK)
    {
        return Y_OK;
    }
    MPU6050::i2cBusReset();
    return Y_ERROR;
}

/**
  * @brief  MPU6050初始化（手册7.4节推荐上电流程）
  */
StatusTypeDef_Y MPU6050::init()
{
    uint8_t who_am_i;

    // 复位设备（REG_PWR_MGMT_1第7位置1）
    if (I2C_Write(REG_PWR_MGMT_1, 0x80) != 0) return Y_ERROR;
    HAL_Delay(100); // 手册要求复位后稳定100ms

    // 检测设备ID
    if (I2C_Read(REG_WHO_AM_I, &who_am_i, 1) != 0 || who_am_i != MPU6050_WHO_AM_I_VAL) {
        return Y_ERROR;
    }

    // 配置寄存器（按手册时序，每步等待5ms稳定）
    I2C_Write(REG_SMPLRT_DIV, 0x02);    // 采样率=1000/(2+1)≈333Hz
    HAL_Delay(5);
    I2C_Write(REG_CONFIG, 0x03);      // DLPF=3（42Hz带宽，低通滤波）
    HAL_Delay(5);
    I2C_Write(REG_GYRO_CONFIG, 0x18); // 陀螺仪±2000°/s（FS_SEL=3）
    HAL_Delay(5);
    I2C_Write(REG_ACCEL_CONFIG, 0x00); // 加速度计±2G（AFS_SEL=0）
    HAL_Delay(5);
    I2C_Write(REG_PWR_MGMT_1, 0x03);  // 时钟源=陀螺仪Z轴（手册推荐提升稳定性）
    HAL_Delay(5);

    return Y_OK;
}

/**
 * @brief  读取数据+滤波+单位转换
 */
void MPU6050::readData()
{
    static uint8_t raw_data[MPU6050_DATA_LEN];
    if (I2C_Read(MPU6050_REG_DATA_START,raw_data,MPU6050_DATA_LEN) != Y_OK )
    {
        return;
    }

    data.acc_raw[0] = (int16_t)(raw_data[0] << 8 | raw_data[1]);
    data.acc_raw[1] = (int16_t)(raw_data[2] << 8 | raw_data[3]);
    data.acc_raw[2] = (int16_t)(raw_data[4] << 8 | raw_data[5]);
    data.temp_raw   = (int16_t)(raw_data[6] << 8 | raw_data[7]);
    data.gyro_raw[0] = (int16_t)(raw_data[8] << 8 | raw_data[9]);
    data.gyro_raw[1] = (int16_t)(raw_data[10] << 8 | raw_data[11]);
    data.gyro_raw[2] = (int16_t)(raw_data[12] << 8 | raw_data[13]);

    data.acc[0]  = (float)data.acc_raw[0] / ACC_FS_2G_LSB;
    data.acc[1]  = -(float)data.acc_raw[1] / ACC_FS_2G_LSB;
    data.acc[2]  = (float)data.acc_raw[2] / ACC_FS_2G_LSB;
    data.gyro[0] = -(float)data.gyro_raw[0] / GYRO_FS_2000DPS_LSB;
    data.gyro[1] = (float)data.gyro_raw[1] / GYRO_FS_2000DPS_LSB;
    data.gyro[2] = -(float)data.gyro_raw[2] / GYRO_FS_2000DPS_LSB;
    // 5. 温度转换（手册6.3节公式：T(℃)=TEMP_OUT/340 + 36.53）
    data.temperature = (float)data.temp_raw / 340.0f + 36.53f;

}

void MPU6050::i2cBusReset()
{
    if (_hi2c->Instance == I2C1) {
        __HAL_RCC_I2C1_FORCE_RESET();
        HAL_Delay(10);
        __HAL_RCC_I2C1_RELEASE_RESET();
    }

    HAL_I2C_DeInit(_hi2c);
    HAL_I2C_Init(_hi2c);
    HAL_Delay(10);
}

// /**
//   * @brief  读取数据+滤波+单位转换
//   */
// void MPU6050_ReadData(void) {
//     uint8_t raw_data[MPU6050_DATA_LEN];
//     static struct _1_ekf_filter acc_ekf[3] = { // 卡尔曼滤波结构体
//         {0.02f, 0, ACC_FS_4G_LSB, 0, 0.001f, 0.543f},
//         {0.02f, 0, ACC_FS_4G_LSB, 0, 0.001f, 0.543f},
//         {0.02f, 0, ACC_FS_4G_LSB, 0, 0.001f, 0.543f}
//     };
//     static float gyro_lpf[3] = {0};
//     const float lpf_factor = 0.25f; // 低通滤波系数
//
//     // 1. 读取原始数据
//     if (MPU6050_I2C_Read(MPU6050_REG_DATA_START, raw_data, MPU6050_DATA_LEN) != 0) {
//         return;
//     }
//
//     // 2. 组合16位数据（手册6.17节：高字节在前）
//     MPU6050.acc_raw[0] = (int16_t)(raw_data[0] << 8 | raw_data[1]);
//     MPU6050.acc_raw[1] = (int16_t)(raw_data[2] << 8 | raw_data[3]);
//     MPU6050.acc_raw[2] = (int16_t)(raw_data[4] << 8 | raw_data[5]);
//     MPU6050.temp_raw   = (int16_t)(raw_data[6] << 8 | raw_data[7]);
//     MPU6050.gyro_raw[0] = (int16_t)(raw_data[8] << 8 | raw_data[9]);
//     MPU6050.gyro_raw[1] = (int16_t)(raw_data[10] << 8 | raw_data[11]);
//     MPU6050.gyro_raw[2] = (int16_t)(raw_data[12] << 8 | raw_data[13]);
//
//     // 3. 加速度计：卡尔曼滤波 + 单位转换（LSB→g）
//     for (uint8_t i = 0; i < 3; i++) {
//         kalman_1(&acc_ekf[i], (float)MPU6050.acc_raw[i]);
//         MPU6050.acc[i] = acc_ekf[i].out / ACC_FS_4G_LSB;
//     }
//
//     // 4. 陀螺仪：一阶低通滤波 + 单位转换（LSB→°/s）
//     for (uint8_t i = 0; i < 3; i++) {
//         gyro_lpf[i] = gyro_lpf[i] * (1 - lpf_factor) + MPU6050.gyro_raw[i] * lpf_factor;
//         MPU6050.gyro[i] = gyro_lpf[i] / GYRO_FS_2000DPS_LSB;
//     }
//
//     // 5. 温度转换（手册6.3节公式：T(℃)=TEMP_OUT/340 + 36.53）
//     MPU6050.temperature = (float)MPU6050.temp_raw / 340.0f + 36.53f;
// }