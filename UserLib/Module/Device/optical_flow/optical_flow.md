## 光流传感器向下: 左-右+ 上-下+
## 采用手动DMA接收,dma一次搬运32字节到缓冲区,光流字节解析状态机找到想要的9个字节,再开始下一次DMA
```c++
/* USER CODE BEGIN PV */
// 定义 DMA 接收缓冲区，一次收32字节
#define DMA_BUF_SIZE 32
uint8_t dma_rx_buf[DMA_BUF_SIZE];
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
flow_init(); // 初始化光流模块

// 启动第一次 DMA 接收
HAL_UART_Receive_DMA(&huart1, dma_rx_buf, DMA_BUF_SIZE);
/* USER CODE END 2 */
```

```c++
/* USER CODE BEGIN 1 */
#include "optical_flow.h"
// 引用 main.c 里的缓冲区
extern uint8_t dma_rx_buf[];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        // 1. DMA 收满了 32 个字节，开始处理
        // 逐个字节喂给状态机解析
        for(int i = 0; i < DMA_BUF_SIZE; i++)
        {
            flow_input(dma_rx_buf[i]); 
        }
        
        // 2. 处理完毕，手动开启下一次 DMA 接收
        // 注意：如果传感器还在发数据，这期间的数据会丢失
        // 但对于你“处理不完”的情况，这是保证数据不乱的唯一办法
        HAL_UART_Receive_DMA(&huart1, dma_rx_buf, DMA_BUF_SIZE);
    }
}
/* USER CODE END 1 */
```

```
DMA (搬运工)：负责高效搬运大块数据（32 字节），减少中断次数。
状态机 (翻译官)：负责在这一大块数据里，精准识别出你要的 9 字节帧。
手动模式：保证状态机处理完这 32 字节之前，不会有新数据覆盖缓冲区。
```