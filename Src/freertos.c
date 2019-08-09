/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "string.h"
#include "stdio.h"
#include "bsp_print.h"
#include "bsp_gsm800a.h"
#include "md5.h"
#include "app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern UART_HandleTypeDef huart1;

/*Define print function*/

int fputc(int ch, FILE* f)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xffffffff);
    return ch;
}

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
osThreadId_t comtaskHandle;
osThreadId_t systemtaskHandle;
osThreadId_t rec_data_taskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void com_task(void *argument);
void system_task(void *argument);
void recdatatask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */
osKernelInitialize();

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
  /* definition and creation of comtask */
  const osThreadAttr_t comtask_attributes = {
    .name = "comtask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 2048
  };
  comtaskHandle = osThreadNew(com_task, NULL, &comtask_attributes);

  /* definition and creation of systemtask */
  const osThreadAttr_t systemtask_attributes = {
    .name = "systemtask",
    .priority = (osPriority_t) osPriorityNormal4,
    .stack_size = 2048
  };
  systemtaskHandle = osThreadNew(system_task, NULL, &systemtask_attributes);

  /* definition and creation of rec_data_task */
  const osThreadAttr_t rec_data_task_attributes = {
    .name = "rec_data_task",
    .priority = (osPriority_t) osPriorityAboveNormal,
    .stack_size = 2048
  };
  rec_data_taskHandle = osThreadNew(recdatatask, NULL, &rec_data_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_com_task */
/**
  * @brief  Function implementing the comtask thread.
  * @param  argument: Not used 
  * @retval None
  */

/* USER CODE END Header_com_task */
void com_task(void *argument)
{
    
    
  /* USER CODE BEGIN com_task */
	    
//    	 uint8_t pcWriteBuffer[500];

  /* Infinite loop */
  for(;;)
  {
    //5秒检测一下是否发送成功


		
    osDelay(1);
		
  }
  /* USER CODE END com_task */
}

/* USER CODE BEGIN Header_system_task */
uint8_t HTTPinit=0;
/**
* @brief Function implementing the systemtask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_system_task */
void system_task(void *argument)
{
  /* USER CODE BEGIN system_task */
     uint8_t a;

  /* Infinite loop */
  for(;;)
  {
     if(HTTPinit==0)
		 {
	
			 printf("nihao");
   	 GSM_HTTP_INIT();
			System_init_param_init();
			 HTTPinit=1;
		 }
   System_ilde_task();

    osDelay(1);
  }
  /* USER CODE END system_task */
}

/* USER CODE BEGIN Header_recdatatask */
extern uint8_t  sendcomd_flag;
/**
* @brief Function implementing the rec_data_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_recdatatask */
void recdatatask(void *argument)
{

  /* USER CODE BEGIN recdatatask */

  /* Infinite loop */
  for(;;)
  {
		//放所有接收数据
	  if(HTTPinit==1&&sendcomd_flag==2)
		{
        all_usart_rec_data();
		}

    osDelay(1);
  }
  /* USER CODE END recdatatask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
