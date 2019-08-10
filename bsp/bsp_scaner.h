#ifndef   __BSP_SCANER_H__
#define   __BSP_SCANER_H__




#include "stm32f4xx_hal.h"




 uint8_t  u2_unpack(uint8_t *dest);  //不能定义 uint8_t state  ，否则传不了参数



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void test_zhuc();

void test_zhu1c();

void DMA_USART2_IDLE_INIT();

 uint8_t  u2_unpack(uint8_t *dest);  //不能定义 uint8_t state  ，否则传不了参数


void u2_data_memy(uint8_t *dest,uint8_t *len);

#endif

