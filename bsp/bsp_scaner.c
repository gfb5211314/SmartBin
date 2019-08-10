#include "bsp_scaner.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "dma.h"
#include "string.h"
#include <stdlib.h>
#include "bsp_print.h"
#include "stdio.h"
#include "bsp_esp8266.h"
extern UART_HandleTypeDef huart2;
USART_RECEIVETYPE  UsartType2;

void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
 UsartReceive_IDLE(&huart2,&UsartType2);
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
  /* USER CODE END USART2_IRQn 1 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{




}
void test_zhuc()
{
	
  esp_8266_connect_web(5,UsartType2.RX_flag,UsartType2.RX_pData);
}
void test_zhu1c()
{
	
  esp_8266_connect_web(9,UsartType2.RX_flag,UsartType2.RX_pData);
}
void DMA_USART2_IDLE_INIT()
{

//	DMA_START(&UsartType2,&huart2);
    DMA_START(&UsartType2,&huart2);

}

 uint8_t  u2_unpack(uint8_t *dest)  //不能定义 uint8_t state  ，否则传不了参数
{
	uint8_t u2_state=0;
	 
	 if(UsartType2.RX_flag)
	 {
		 printf("123");
			printf("%s\r\n",UsartType2.RX_pData);
   memcpy(dest,UsartType2.RX_pData, UsartType2.RX_Size);
		  UsartType2.RX_flag=0;
		   u2_state=1;
		   return  u2_state;
	 }
	 	 else
	 {
		  UsartType2.RX_flag=0;
		   u2_state=0;
		   return  u2_state;
	 }	
}
	


void u2_data_memy(uint8_t *dest,uint8_t *len)
{
	
   memcpy(dest,UsartType2.RX_pData, UsartType2.RX_Size);
	 *len=UsartType2.RX_Size;
}