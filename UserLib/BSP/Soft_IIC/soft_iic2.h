//
// Created by DingYong on 2026/2/7.
//
#ifndef SOFT_IIC_H
#define SOFT_IIC_H
/* 头文件 */
#include <stdint.h>

/* 宏定义 */
#define IIC2_GPIO GPIOB
#define SDA2     GPIO_PIN_11
#define SCL2      GPIO_PIN_10

/* 函数 */
void Soft_IIC2_Init(void);
int8_t IIC2_ReadData(uint8_t dev_addr, uint8_t reg_addr, uint8_t *pdata, uint8_t count);
int8_t IIC2_WriteData(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);

#endif //SOFT_IIC_H