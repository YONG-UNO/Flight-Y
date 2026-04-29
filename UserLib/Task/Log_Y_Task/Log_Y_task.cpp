#include "Log_Y_task.hpp"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

extern "C" {
#include "SEGGER_RTT.hpp"
}

float static dis_data[3];

void Log_Y(void const * argument)
{
    // 初始化 RTT
    SEGGER_RTT_Init();

    // 上电延时一下，保证系统稳定
    osDelay(100);

    // 只打印一次表头
    SEGGER_RTT_printf(0, "time,gx,gy,gz\n");

    // for (;;)
    // {
    //     // 获取系统时间
    //     uint32_t time = HAL_GetTick();
    //
    //     // 假数据 → 你后期换成真实陀螺仪数据即可
    //     dis_data[0] = euler[0];
    //     dis_data[1] = euler[1];
    //     dis_data[2] = euler[2];
    //
    //     // RTT 输出 CSV 格式
    //     SEGGER_RTT_printf(0, "%lu,%d,%d,%d\n", time, (int)(dis_data[0]*1000), (int)(dis_data[1]*1000), (int)(dis_data[2]*1000));
    //
    //     // 100ms 输出一次，方便看
    //     osDelay(1);
    // }
}
