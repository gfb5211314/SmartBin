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
#include "iwdg.h"
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
osThreadId_t sensortaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint32_t g_osRuntimeCounter = 0;


/* USER CODE END FunctionPrototypes */

void com_task(void *argument);
void system_task(void *argument);
void recdatatask(void *argument);
void sensor_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
       g_osRuntimeCounter = 0;
}

__weak unsigned long getRunTimeCounterValue(void)
{
   return g_osRuntimeCounter;
}
/* USER CODE END 1 */

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
    .stack_size = 3000
  };
  comtaskHandle = osThreadNew(com_task, NULL, &comtask_attributes);

  /* definition and creation of systemtask */
  const osThreadAttr_t systemtask_attributes = {
    .name = "systemtask",
    .priority = (osPriority_t) osPriorityNormal4,
    .stack_size = 3000
  };
  systemtaskHandle = osThreadNew(system_task, NULL, &systemtask_attributes);

  /* definition and creation of rec_data_task */
  const osThreadAttr_t rec_data_task_attributes = {
    .name = "rec_data_task",
    .priority = (osPriority_t) osPriorityAboveNormal,
    .stack_size = 3000
  };
  rec_data_taskHandle = osThreadNew(recdatatask, NULL, &rec_data_task_attributes);

  /* definition and creation of sensortask */
  const osThreadAttr_t sensortask_attributes = {
    .name = "sensortask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 2048
  };
  sensortaskHandle = osThreadNew(sensor_task, NULL, &sensortask_attributes);

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
    
    
    
    
//    char g_tasks_buf[512]; //用于存放显示数据

  /* USER CODE BEGIN com_task */

//    	 uint8_t pcWriteBuffer[500];

    /* Infinite loop */
    for(;;)
    {
        //5秒检测一下是否发送成功
//             printf("com_task\n");
//            vTaskList((char *)&g_tasks_buf);
//            printf("任务名                任务状态 优先级   剩余栈 任务序号\r\n");
//            printf("%s\r\n", g_tasks_buf);    
//            vTaskGetRunTimeStats((char *)&g_tasks_buf);
//            printf("\r\n任务名            运行计数         使用率\r\n");
//            printf("%s\r\n", g_tasks_buf);    
// 

        resend_task();
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
//char g_tasks_buf[512]; //用于存放显示数据
    /* Infinite loop */
    for(;;)
    {
//			   printf("system_task\n");
//            vTaskList((char *)&g_tasks_buf);
//            printf("任务名               任务状态 优先级   剩余栈 任务序号\r\n");
//            printf("%s\r\n", g_tasks_buf);    

//            vTaskGetRunTimeStats((char *)&g_tasks_buf);
//            printf("\r\n任务名           运行计数         使用率\r\n");
//            printf("%s\r\n", g_tasks_buf);   
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
//char g_tasks_buf[512]; //用于存放显示数据
    /* Infinite loop */
    for(;;)
    {
//			   printf("recdatatask\n");
//            vTaskList((char *)&g_tasks_buf);
//            printf("任务名              任务状态 优先级   剩余栈 任务序号\r\n");
//            printf("%s\r\n", g_tasks_buf);    

//            vTaskGetRunTimeStats((char *)&g_tasks_buf);
//            printf("\r\n任务名          运行计数         使用率\r\n");
//            printf("%s\r\n", g_tasks_buf);   
        IWDG_Feed();
        //放所有接收数据
        if(HTTPinit==1&&sendcomd_flag==2)
        {
            all_usart_rec_data();
        }

        osDelay(1);
    }
  /* USER CODE END recdatatask */
}

/* USER CODE BEGIN Header_sensor_task */
/**
* @brief Function implementing the sensortask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sensor_task */
void sensor_task(void *argument)
{
  /* USER CODE BEGIN sensor_task */
//	char g_tasks_buf[512]; //用于存放显示数据
  /* Infinite loop */
  for(;;)
  {
//		     printf("sensor_task\n");
//            vTaskList((char *)&g_tasks_buf);
//            printf("任务名                任务状态 优先级   剩余栈 任务序号\r\n");
//            printf("%s\r\n", g_tasks_buf);    

//            vTaskGetRunTimeStats((char *)&g_tasks_buf);
//            printf("\r\n任务名            运行计数         使用率\r\n");
//            printf("%s\r\n", g_tasks_buf);   
    osDelay(1);
  }
  /* USER CODE END sensor_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
