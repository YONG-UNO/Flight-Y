//
// Created by DingYong on 2026/4/19.
//

#ifndef NAVIOPTIC_TOF_Y_STATUSTYPEDEF_Y_HPP
#define NAVIOPTIC_TOF_Y_STATUSTYPEDEF_Y_HPP

/**
 * @brief  Y 工程专属状态返回值
 * @note   仿照 ST HAL 标准格式
 * @author Ding Yong
 */
typedef enum
{
    Y_OK       = 0x00U,  /* 操作成功 */
    Y_ERROR    = 0x01U,  /* 操作失败 */
    Y_BUSY     = 0x02U,  /* 设备忙 */
    Y_TIMEOUT  = 0x03U,  /* 超时 */
    Y_NACK     = 0x04U,  /* 无应答 */
    Y_INVALID  = 0x05U,  /* 参数非法 */
    Y_UNREADY  = 0x06U   /* 设备未就绪 */
} StatusTypeDef_Y;

#endif //NAVIOPTIC_TOF_Y_STATUSTYPEDEF_Y_HPP
