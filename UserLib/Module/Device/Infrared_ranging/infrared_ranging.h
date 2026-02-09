//
// Created by DingYong on 2026/2/5.
//
#ifndef INFRARED_RANGING_H
#define INFRARED_RANGING_H

/* 头文件 */
#include <stdint.h>

/* 宏定义 */
#define RANGING_ADDR             0x55   // 测距模块IIC地址(7bit)
#define RANGING_REG_H            0x00   // 距离高字节(mm)
#define RANGING_REG_L            0x01   // 距离低字节(mm)
#define RANGING_REG_STRENGTH     0x02   // 信号强度(0~64)

/* 结构体 */
typedef struct {
    uint16_t distance;  // 距离(mm)
    uint8_t strange;    // 强度(0~64)
} ranging_t;

extern ranging_t ranging_data;

/* 函数 */
void ranging_init(void);    // 初始化
void ranging_read(void);

#endif //INFRARED_RANGING_H