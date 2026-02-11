//
// Created by DingYong on 2026/2/5.
//
#ifndef OPTICAL_FLOW_H
#define OPTICAL_FLOW_H

/* 头文件 */
#include <stdint.h>

/* 结构体 */

// 光流数据结构体
typedef struct {
    int16_t flow_x;       // x轴瞬时位移
    int16_t flow_y;       // y轴瞬时位移
    int16_t flow_x_i;     // x轴累计位移
    int16_t flow_y_i;     // y轴累计位移
    int8_t qual;       // 信号质量(0~255)
} flow_t;

extern flow_t flow; // 光流数据实例


/* 函数 */
void flow_init(void);  // 初始化光流解析模块
void flow_input(uint8_t byte);  // 单字节解析
flow_t *flow_get_data(void);  // 返回光流结构体地址

#endif //OPTICAL_FLOW_H