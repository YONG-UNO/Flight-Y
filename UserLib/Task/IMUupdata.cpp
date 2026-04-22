//
// Created by DingYong on 2026/4/21.
//

#include "IMUupdata.hpp"

#include "MPU6050.hpp"
#include "cmsis_os.h"
#include "i2c.h"

Mahony mahony;
static MPU6050 mpu(&hi2c1);

#define k 0.95f

static float acc[3],gyro[3];
static float euler[3];
static float euler_last[3];
void imuUpdate(void const * argument)
{
    mpu.init();
    for (;;)
    {
        mpu.readData();
        gyro[0] = mpu.data.gyro[0] * DEG_TO_RAD;
        gyro[1] = mpu.data.gyro[1] * DEG_TO_RAD;
        gyro[2] = mpu.data.gyro[2] * DEG_TO_RAD;
        acc[0] = mpu.data.acc[0];
        acc[1] = mpu.data.acc[1];
        acc[2] = mpu.data.acc[2];

        mahony.update(gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2],0,0,0);

        euler[0] = mahony.getRollRadians();
        euler[1] = mahony.getPitchRadians();
        euler[2] = mahony.getYawRadians();

        euler[0] = lowPassFiliter(euler[0],euler_last[0],k);
        euler[1] = lowPassFiliter(euler[1],euler_last[1],k);
        euler[2] = lowPassFiliter(euler[2],euler_last[2],k);

        euler_last[0] = euler[0];
        euler_last[1] = euler[1];
        euler_last[2] = euler[2];

        osDelay(1);
    }

}