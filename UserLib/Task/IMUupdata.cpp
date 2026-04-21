//
// Created by DingYong on 2026/4/21.
//

#include "IMUupdata.hpp"

#include "MPU6050.hpp"
#include "MahonyAHRS.hpp"
#include "cmsis_os.h"
#include "i2c.h"

Mahony mahony;
static MPU6050 mpu(&hi2c1);

static float gx,gy,gz,ax,ay,az;
static float roll,pitch,yaw;
void imuUpdate(void const * argument)
{
    mpu.init();
    for (;;)
    {
        mpu.readData();
        gx = mpu.data.gyro[0];
        gy = mpu.data.gyro[1];
        gz = mpu.data.gyro[2];
        ax = mpu.data.acc[0];
        ay = mpu.data.acc[1];
        az = mpu.data.acc[2];

        mahony.updateIMU(gx,gy,gz,ax,ay,az);

        roll = mahony.getRollRadians();
        pitch = mahony.getPitchRadians();
        yaw = mahony.getYawRadians();

        osDelay(1);
    }

}