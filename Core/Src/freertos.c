/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for INTEGRATION */
osThreadId_t INTEGRATIONHandle;
const osThreadAttr_t INTEGRATION_attributes = {
  .name = "INTEGRATION",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for CONTROL_PITCH */
osThreadId_t CONTROL_PITCHHandle;
const osThreadAttr_t CONTROL_PITCH_attributes = {
  .name = "CONTROL_PITCH",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime7,
};
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Integration(void *argument);
void Control_Pitch(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of INTEGRATION */
  INTEGRATIONHandle = osThreadNew(Integration, NULL, &INTEGRATION_attributes);

  /* creation of CONTROL_PITCH */
  CONTROL_PITCHHandle = osThreadNew(Control_Pitch, NULL, &CONTROL_PITCH_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Integration */
/**
  * @brief  Function implementing the INTEGRATION thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Integration */
__weak void Integration(void *argument)
{
  /* USER CODE BEGIN Integration */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Integration */
}

/* USER CODE BEGIN Header_Control_Pitch */
/**
* @brief Function implementing the CONTROL_PITCH thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Control_Pitch */
__weak void Control_Pitch(void *argument)
{
  /* USER CODE BEGIN Control_Pitch */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Control_Pitch */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

