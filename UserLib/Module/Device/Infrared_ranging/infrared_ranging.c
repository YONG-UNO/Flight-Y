//
// Created by DingYong on 2026/2/5.
//
/* 头文件 */
#include "soft_iic2.h"
#include "infrared_ranging.h"


/* 结构体+数组 */
ranging_t ranging_data = {0};   // 测距结果
uint8_t rx_buf[3] = {0};   // 接收缓存:高8位,低8位,强度


/* 函数 */


void ranging_init(void) {
    ranging_data.distance = 0;
    ranging_data.strange  = 0;
}

// 单次读取测距数据：软件IIC实现，读取3字节并解析
// 替代原硬件I2C的中断+DMA流程，直接调用即可获取数据
void ranging_read(void) {
    // 用软件IIC多字节读：从0x55的0x00寄存器读取3字节到rx_buf
    // 复用写好的IIC_ReadData，直接适配，无需再写底层时序
    if(IIC2_ReadData(RANGING_ADDR, RANGING_REG_H, rx_buf, 3) == 0)
    {
        // 读取成功，解析数据（和原代码逻辑完全一致）
        ranging_data.distance = (uint16_t)rx_buf[0] << 8 | rx_buf[1];
        ranging_data.strange = rx_buf[2];
    }
    else
    {
        // 读取失败（无ACK/超时），数据清零，避免脏数据
        ranging_data.distance = 0;
        ranging_data.strange  = 0;
    }
}