//
// Created by DingYong on 2026/4/21.
//

#ifndef FLIGHT_Y_IMUUPDAT_HPP
#define FLIGHT_Y_IMUUPDAT_HPP
#include "MahonyAHRS.hpp"

extern Mahony mahony;
extern float euler[3];

extern "C" {

void imuUpdate(void const * argument);
inline float lowPassFiliter(float &current,float &last,float k)
{
    return ((1-k) * current + k * last);
}
}

#endif //FLIGHT_Y_IMUUPDAT_HPP
