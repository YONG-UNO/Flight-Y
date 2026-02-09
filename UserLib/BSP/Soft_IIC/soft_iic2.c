//
// Created by DingYong on 2026/2/7.
//

/* 头文件 */
#include "soft_iic2.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

/* 宏定义 */
#define IIC2_SDA_H() HAL_GPIO_WritePin(IIC2_GPIO, SDA2, GPIO_PIN_SET)
#define IIC2_SDA_L() HAL_GPIO_WritePin(IIC2_GPIO, SDA2, GPIO_PIN_RESET)
#define IIC2_SCL_H() HAL_GPIO_WritePin(IIC2_GPIO, SCL2, GPIO_PIN_SET)
#define IIC2_SCL_L() HAL_GPIO_WritePin(IIC2_GPIO, SCL2, GPIO_PIN_RESET)
#define IIC2_SDA_Read() HAL_GPIO_ReadPin(IIC2_GPIO, SDA2)

/* 函数 */
void IIC2_Delay(uint16_t time) {
    uint16_t count;
    count = time * 6;
    while (count --);
}

// IIC_SDA2引脚配置为输出模式
void IIC2_SDA_Out(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // 定义GPIO初始化结构体并清零
    GPIO_InitStruct.Pin = SDA2; // 指定配置的引脚,SDA2:GPIO_PIN_11
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP; // 上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
    HAL_GPIO_Init(IIC2_GPIO, &GPIO_InitStruct); // 调用HAL库初始化指定GPIO
}

// IIC_SDA2引脚配置为输入模式(开漏输入 + 上拉/无拉)
void IIC2_SDA_In(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // 定义GPIO初始化结构体并清零
    GPIO_InitStruct.Pin = SDA2; // 指定配置的引脚,SDA2:GPIO_PIN_11
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // 普通输入模式
    GPIO_InitStruct.Pull = GPIO_PULLUP; // 上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
    HAL_GPIO_Init(IIC2_GPIO, &GPIO_InitStruct); // 调用HAL库初始化指定GPIO
}

void IIC2_Start(void)
{
    IIC2_SDA_Out();      // 强制SDA2为输出模式，MCU主动控制电平（必须）
    IIC2_SDA_H();        // SDA2先置高，为下降沿做准备
    IIC2_SCL_H();        // SCL2置高，满足「SCL2高电平期间SDA2跳变」的时序要求
    IIC2_Delay(1);  // 延时，让总线稳定在高电平，给从机识别时间
    IIC2_SDA_L();        // SCL2高时，SDA2拉低 → 产生I2C起始信号的下降沿（核心）
    IIC2_Delay(1);  // 延时，保证起始信号的低电平保持时间，从机稳定识别
    IIC2_SCL_L();        // SCL2拉低，结束起始信号，进入**数据传输阶段**（SCL2低时才允许SDA2跳变）
}

void IIC2_Stop(void)
{
    IIC2_SDA_Out();      // 强制SDA2为输出模式，MCU主动控制电平（必须）
    IIC2_SCL_L();        // SCL2先置低，满足「SCL2低时SDA2可跳变」的规范，为上升沿做准备
    IIC2_SDA_L();        // SDA2拉低，为上升沿做准备
    IIC2_Delay(1);  // 延时，让总线稳定在低电平
    IIC2_SCL_H();        // SCL2置高，进入「SCL2高电平期间」
    IIC2_SDA_H();        // SCL2高时，SDA2拉高 → 产生I2C停止信号的上升沿（核心）
    IIC2_Delay(1);  // 延时，保证停止信号的高电平保持时间，从机稳定识别
}

// 主机主动发送应答 / 非应答信号
void IIC2_Ack(uint8_t re)
{
    IIC2_SDA_Out();  // 1. 切SDA2为输出，主机主动控制应答电平（必须）
    if(re)
        IIC2_SDA_H();// 2. re=1 → SDA2拉高 → 发送NACK（非应答，告诉从机停止发送）
    else
        IIC2_SDA_L();// 3. re=0 → SDA2拉低 → 发送ACK（应答，告诉从机继续发送）
    IIC2_SCL_H();   // 4. SCL2拉高 → 从机采样SDA2电平（第9位的核心采样时刻）
    IIC2_Delay(1);  // 5. 延时，保证采样时间，让从机稳定识别
    IIC2_SCL_L();   // 6. SCL2拉低 → 结束应答位，准备下一次通信
    IIC2_Delay(1);  // 7. 延时，稳定总线
}

// 主机等待并检测从机的应答信号
int8_t IIC2_WaitAck(void)
{
    uint16_t Out_Time=1000;   // 定义超时计数器，初始值1000（防死等）

    IIC2_SDA_In();             // 切SDA2为输入，高阻态,准备读取从机应答
    IIC2_Delay(1);        // 延时稳定
    IIC2_SCL_H();              // SCL2拉高，采样SDA2电平（第9位采样时刻）
    IIC2_Delay(1);        // 延时保证采样时间
    while(IIC2_SDA_Read())     // 循环检测SDA2：高电平则继续等
    {
        if(--Out_Time == 0)        // 超时计数器自减
        {
            IIC2_Stop();       // 超时后发送停止信号，终止通信
            return -1;      // 返回超时标志
        }
    }
    IIC2_SCL_L();              // 检测到ACK（SDA2低），拉低SCL2结束应答位
    return 0;                 // 返回ACK成功标志
}


/**
  * @brief  I2C底层单字节发送函数：将1个字节按高位先行逐位发送到SDA2总线
  * @param  Temp: 要发送的1字节数据（uint8_t）
  * @retval 无
  * @note   发送前自动将SDA2置为输出模式，全程由主机控制SDA2/SCL2电平
  * @note   时序规则：SCL2拉低准备SDA2电平→置SDA2→延时稳定→SCL2拉高让从机采样→拉低SCL2完成1位
  */
void IIC2_WriteByte(uint8_t Temp)
{
    uint8_t i;                // 循环计数器，遍历8个位
    IIC2_SDA_Out();            // 发送前强制SDA2为输出，主机主动控制电平
    IIC2_SCL_L();              // 先拉低SCL2，I2C规定：SCL2低电平时允许SDA2电平变化
    for(i=0;i<8;i++)          // 逐位发送8个比特，高位先行（I2C协议要求）
    {
        if(Temp&0x80)         // 检测最高位（0x80=1000 0000），判断当前发送位是1还是0
        {
            IIC2_SDA_H();      // 最高位为1，SDA2置高
        }
        else
        {
            IIC2_SDA_L();      // 最高位为0，SDA2拉低
        }
        Temp<<=1;             // 数据左移1位，将次高位变为新的最高位，准备下一位发送
        IIC2_Delay(1);         // 延时1us，让SDA2电平稳定，避免从机采样到不稳定电平
        IIC2_SCL_H();          // 拉高SCL2，从机检测到SCL2上升沿后，采样当前SDA2电平
        IIC2_Delay(1);         // 延时1us，保证从机有足够时间完成采样
        IIC2_SCL_L();          // 拉低SCL2，完成1位数据发送，准备下一位
    }
}

/**
  * @brief  I2C底层单字节接收函数：从SDA2总线按高位先行逐位读取1个字节
  * @retval Temp: 读取到的1字节数据（uint8_t）
  * @note   接收前自动将SDA2置为输入模式，释放总线由从机控制SDA2电平
  * @note   时序规则：SCL2拉低让从机准备SDA2→拉高SCL2主机采样→拼接数据→拉低SCL2完成1位
  */
uint8_t IIC2_ReadByte(void)
{
    uint8_t i,Temp=0;         // i：循环计数器；Temp：接收数据缓存，初始化为0
    IIC2_SDA_In();             // 接收前强制SDA2为输入，主机放弃控制权，由从机控制SDA2
    for(i=0;i<8;i++)          // 逐位读取8个比特，高位先行（I2C协议要求）
    {
        IIC2_SCL_L();          // 拉低SCL2，通知从机准备下1位数据（从机在SCL2低时切换SDA2）
        IIC2_Delay(1);         // 延时1us，让从机有足够时间准备好SDA2电平
        IIC2_SCL_H();          // 拉高SCL2，主机检测到SCL2上升沿后，采样当前SDA2电平
        Temp<<=1;             // 缓存数据左移1位，为接收当前位腾出最低位
        if(IIC2_SDA_Read())    // 读取SDA2电平：高电平为1，低电平为0
        {
            Temp++;            // SDA2为1，缓存数据最低位置1；为0则不操作，保持0
        }
        IIC2_Delay(1);         // 延时1us，保证采样稳定，避免电平抖动影响结果
    }
    IIC2_SCL_L();              // 所有位读取完成，拉低SCL2，稳定总线状态
    return Temp;              // 返回拼接后的完整1字节数据
}

/**
  * @brief  I2C单字节写寄存器：向指定从机的指定寄存器写入1个字节数据
  * @param  dev_addr: 从机7位I2C地址（无需带读写位，函数内自动拼接）
  * @param  reg_addr: 从机内部寄存器地址
  * @param  data:     要写入寄存器的1字节数据
  * @retval 0: 写入成功; -1: 写入失败（某一步未收到从机ACK，超时）
  * @note   严格遵循I2C写协议，每发送1个字节后检测从机应答，异常则直接返回
  */
int8_t IIC2_WriteData(uint8_t dev_addr,uint8_t reg_addr,uint8_t data)
{
    IIC2_Start();  // 生成I2C起始信号，启动一次I2C通信

    // 发送从机地址+写位：7位地址左移1位，最低位置0表示写操作（I2C协议要求）
    IIC2_WriteByte((dev_addr << 1) | 0);
    if(IIC2_WaitAck() == -1)  // 等待从机应答，超时则返回失败
    {
        IIC2_Stop();
        return -1;
    }

    // 发送要写入的从机内部寄存器地址
    IIC2_WriteByte(reg_addr);
    if(IIC2_WaitAck() == -1)  // 等待寄存器地址应答，超时返回失败
    {
        IIC2_Stop();
        return -1;
    }

    // 发送要写入寄存器的1字节数据
    IIC2_WriteByte(data);
    if(IIC2_WaitAck() == -1)  // 等待数据应答，超时返回失败
    {
        IIC2_Stop();
        return -1;
    }

    IIC2_Stop();   // 生成I2C停止信号，结束本次I2C写通信
    return 0;     // 所有步骤均收到ACK，写入成功
}

/**
  * @brief  I2C多字节读寄存器：从指定从机的指定寄存器读取多个字节数据
  * @param  dev_addr: 从机7位I2C地址（无需带读写位，函数内自动拼接）
  * @param  reg_addr: 从机内部要读取的寄存器起始地址
  * @param  pdata:    数据接收缓冲区指针（需提前开辟内存，存放读取到的数据）
  * @param  count:    要读取的字节数（大于等于1）
  * @retval 0: 读取成功; -1: 读取失败（某一步未收到从机ACK，超时）
  * @note   严格遵循I2C读协议：先写(从机地址+寄存器地址)，再重复起始→读(从机地址+数据)
  * @note   连续读应答规则：前count-1个字节发ACK，最后1个字节发NACK，告诉从机停止发送
  */
int8_t IIC2_ReadData(uint8_t dev_addr, uint8_t reg_addr, uint8_t * pdata, uint8_t count)
{
    uint8_t i;          // 循环计数器，控制读取字节数
    IIC2_Start();        // 1. 生成第一次起始信号，启动I2C通信（写阶段）

    // 发送【从机7位地址+写位】：左移1位后最低位置0，表示写操作（先写地址阶段）
    // 原BUG1：直接传dev_addr，缺少读写位，从机无法识别操作类型
    IIC2_WriteByte((dev_addr << 1) | 0);
    if(IIC2_WaitAck() == -1)  // 等待从机应答，超时则直接返回失败
    {
        IIC2_Stop();
        return -1;
    }

    // 发送要读取的【从机内部寄存器起始地址】
    IIC2_WriteByte(reg_addr);
    if(IIC2_WaitAck() == -1)  // 等待寄存器地址应答，超时返回失败
    {
        IIC2_Stop();
        return -1;
    }

    IIC2_Start();        // 2. 生成重复起始信号（I2C读协议要求，切换为读阶段）

    // 发送【从机7位地址+读位】：左移1位后最低位置1，表示读操作
    // 原BUG2：用dev_addr+1拼接，会导致地址错位（如0x48→0x49，而非正确的0x91）
    IIC2_WriteByte((dev_addr << 1) | 1);
    if(IIC2_WaitAck() == -1)  // 等待读地址应答，超时返回失败
    {
        IIC2_Stop();
        return -1;
    }

    // 3. 循环读取count个字节数据
    for(i=0; i<(count-1); i++)  // 读取前count-1个字节
    {
        *pdata = IIC2_ReadByte(); // 读取1个字节数据，存入接收缓冲区
        IIC2_Ack(0);             // 发送ACK（低电平），告诉从机继续发送下一个字节
        pdata++;                // 缓冲区指针后移，准备接收下一个字节
    }

    // 读取最后1个字节
    *pdata = IIC2_ReadByte();     // 读取最后1个字节数据
    IIC2_Ack(1);                 // 发送NACK（高电平），告诉从机停止发送数据

    IIC2_Stop();                 // 4. 生成停止信号，结束本次I2C读通信
    return 0;                   // 所有步骤均收到ACK，读取成功
}

/**
  * @brief  IIC总线全局初始化
  * @note   主函数上电后必须先调用，初始化SCL2引脚+使能GPIO时钟+总线空闲
  * @retval 无
  */
void Soft_IIC2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();  // 使能GPIOB时钟

    // 初始化SCL2：推挽输出 + 开启内部上拉（核心修改）
    GPIO_InitStruct.Pin = SCL2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // 从NOPULL改为PULLUP，开启内部上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC2_GPIO, &GPIO_InitStruct);

    // 初始化SDA2：输出+上拉+置高（核心修改）
    IIC2_SDA_Out();
    GPIO_InitStruct.Pin = SDA2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // SDA2也开启内部上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC2_GPIO, &GPIO_InitStruct);

    IIC2_SDA_H();
    IIC2_SCL_H();    // 总线空闲状态：SDA2/SCL2均为高
}