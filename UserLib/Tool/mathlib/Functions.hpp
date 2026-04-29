//
// Created by DingYong on 2026/4/14.
//

#ifndef FLIGHT_Y_FUNCTIONS_H
#define FLIGHT_Y_FUNCTIONS_H
class math
{
public:
    static float constrain(float val ,float min_val,float max_val)
    {
        return (val < min_val) ? min_val : ((val > max_val) ? max_val : val);
    }
};

#endif //FLIGHT_Y_FUNCTIONS_H
