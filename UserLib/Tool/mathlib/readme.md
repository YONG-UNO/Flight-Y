```c++
#include "mathlib.hpp"

// 创建卡尔曼实例
math::kalman::Filter kf;

// 初始化
math::kalman::init(&kf, 0.01f, 0.5f);

// 滤波
float value = math::kalman::update(&kf, raw_data);

// 限幅
float output = math::constrain(value, 0, 1);
```