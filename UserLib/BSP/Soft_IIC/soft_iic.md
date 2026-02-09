## CubeMX 只需将SCL和SDA配置成 推挽输出
```c++
单字节写函数 IIC_WriteData
int8_t IIC_WriteData(从机7位地址, 寄存器地址, 要写入的单字节数据);
返回值：0= 成功，0xff= 失败；
示例：IIC_WriteData(0x48, 0x00, 0x5A);

多字节读函数 IIC_ReadData
int8_t IIC_ReadData(从机7位地址, 寄存器地址, 接收缓冲区指针, 要读取的字节数);
返回值：0= 成功，0xff= 失败；
关键：接收缓冲区必须是指针 / 数组（让函数能把数据存进去）；
单字节读复用：IIC_ReadData(0x48, 0x00, &data, 1);（用&取单个变量的地址）
```  