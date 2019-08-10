#ifndef  __BSP_PRINT_H__
#define  __BSP_PRINT_H__

#include "stm32f4xx_hal.h"

#include "common_usart.h"
#include "usart.h"





extern  void DMA_USART_IDLE_INIT(void);

extern void UsartReceive_IDLE(UART_HandleTypeDef *huart,USART_RECEIVETYPE   *Usart);

  void  DMA_START(USART_RECEIVETYPE  *Usart,UART_HandleTypeDef *huart);

void DMA_USART3_IDLE_INIT(void);
#endif