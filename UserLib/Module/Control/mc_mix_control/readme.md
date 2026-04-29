```c++
// 实例化
MultirotorMixer mixer;

// 角速度PID输出力矩
matrix::Vector3f torque = rate_controller.run(...);

// 总油门（来自遥控器）
float throttle = 0.5f;

// 电机输出数组
float motors[MAX_MOTORS];

// 执行混控
mixer.mixQuadX(torque, throttle, motors);
```
````
motors[0] 前左
motors[1] 前右
motors[2] 后右
motors[3] 后左
````