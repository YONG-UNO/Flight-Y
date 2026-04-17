//
// Created by DingYong on 2026/4/14.
//

#include "Functions.h"

inline float constrain(float val ,float min_val,float max_val) {
    return (val < min_val) ? min_val : ((val > max_val) ? max_val : val);
}
