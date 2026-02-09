//
// Created by DingYong on 2026/2/5.
//
#include "optical_flow.h"

/* 宏定义 */
#define FRAME_HEAD      0xFE   // 帧头
#define FUNC_CODE       0x04   // 功能码
#define FRAME_LEN       9      // 一帧总长度

/* 结构体 */
// 缓冲区和状态
typedef struct {
    uint8_t buf[9];
    uint8_t cnt;
    uint8_t state;
}rx_t;

rx_t rx = {0};     // 接收状态实例(内部,静态)
flow_t flow = {0}; // 光流数据实例(内部,静态)

/* 函数 */

/**
 * @brief  初始化光流解析模块
 * @note   重置接收状态与数据
 * @retval 无
 */
void flow_init(void) {
    rx.cnt = 0;
    rx.state = 0;

    flow.flow_x = 0;
    flow.flow_y = 0;
    flow.flow_x_i = 0;
    flow.flow_y_i = 0;
    flow.qual = 0;
}

/**
 * @brief  输入一个字节并驱动状态机解析
 * @param  byte: 串口收到的单字节数据
 * @retval 无
 */
void flow_input(uint8_t byte) {
    switch (rx.state) {
        case 0: // 等待帧头0xFE
            if (byte == FRAME_HEAD) {
                rx.state = 1;
                rx.buf[rx.cnt++] = byte;
            }
            break;

        case 1: // 等待功能码0x04
            if (byte == FUNC_CODE) {
                rx.state = 2;
                rx.buf[rx.cnt++] = byte;
            }else {
                rx.state = 0; // 错误,重开
                rx.cnt = 0;
            }
            break;

        case 2: // 接收数据
            rx.buf[rx.cnt++] = byte;

            if (rx.cnt == FRAME_LEN) { // 收满一帧
                rx.state = 0;
                rx.cnt = 0;

                // 校验和验证
                uint8_t sum = rx.buf[2] + rx.buf[3] + rx.buf[4] + rx.buf[5];
                if (sum == rx.buf[6]) {
                    // 解析数据
                    flow.flow_x = -((rx.buf[3] << 8) | rx.buf[2]);
                    flow.flow_y = -((rx.buf[5] << 8) | rx.buf[4]);
                    flow.qual = rx.buf[7];

                    // 累计位移
                    flow.flow_x_i += flow.flow_x;
                    flow.flow_y_i += flow.flow_y;
                }
            }
            break;

        default:  // 异常重置
            rx.state = 0;
            rx.cnt = 0;
            break;
    }
}

/**
 * @brief  获取光流数据指针（外部只读接口,封装,只需修改返回的结构体指针,避免全局结构体修改,其他一起修改）
 * @retval flow_t*: 光流数据结构体指针
 */
flow_t *flow_get_data(void) {
    return &flow;
}