#include "bsp_print.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "dma.h"
#include "string.h"
#include <stdlib.h>
#include "stdio.h"






extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;
USART_RECEIVETYPE  UsartType3;

/*Define print function*/






/**************DMA_START_receive***********************/
  void  DMA_START(USART_RECEIVETYPE  *Usart,UART_HandleTypeDef *huart)
{

    HAL_UART_Receive_DMA(huart,(uint8_t *)Usart->RX_pData, RX_LEN);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
}
/*********receive data*******************/
void UsartReceive_IDLE(UART_HandleTypeDef *huart,USART_RECEIVETYPE   *Usart)
{
    uint32_t temp=0;

    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
    {



        __HAL_UART_CLEAR_IDLEFLAG(huart);
        HAL_UART_DMAStop(huart);
        temp = huart->hdmarx->Instance->NDTR;//
        Usart->RX_Size =  RX_LEN - temp;
        Usart->RX_flag=1;
        HAL_UART_Receive_DMA(huart,(uint8_t *)Usart->RX_pData,RX_LEN);

    }
}


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if(huart==&huart3)
//		{
//   UsartReceive_IDLE(&huart3,&UsartType3);
//		}
//		 if(huart==&huart2)
//		{
//   UsartReceive_IDLE(&huart2,&UsartType3);
//		}
//		 if(huart==&huart6)
//		{
//   UsartReceive_IDLE(&huart6,&UsartType3);
//		}



//}
void DMA_USART3_IDLE_INIT()
{

//	DMA_START(&UsartType2,&huart2);
   	DMA_START(&UsartType3,&huart3);


}
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
UsartReceive_IDLE(&huart3,&UsartType3);
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
 * @brief print_qr_data 打印函数封装
 * @param pbuf 打印二维码的数据
 * @return void
 * @retval void
 */
void print_qr_data(char *pbuf,uint8_t len)
{
	   uint8_t index=0;
	   uint8_t print_qr_data_buf[200];
	/*****start****************/
	  print_qr_data_buf[index++]=0x1B; 
		print_qr_data_buf[index++]=0x40; 
		print_qr_data_buf[index++]=0x1A; 
		print_qr_data_buf[index++]=0x5B; 
		print_qr_data_buf[index++]=0x01; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x80;
		print_qr_data_buf[index++]=0x01; 
			print_qr_data_buf[index++]=0xF0; 
	  print_qr_data_buf[index++]=0x00;
	print_qr_data_buf[index++]=0x00;
	/********************************/
	
		print_qr_data_buf[index++]=0x1A; 
		print_qr_data_buf[index++]=0x31; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x01; 
		print_qr_data_buf[index++]=0x04; 
		print_qr_data_buf[index++]=0x64; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x0A; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x06;
		print_qr_data_buf[index++]=0x00; 
		/****QRdata***************/
		for(uint8_t i=0;i<len;i++)
		{
			print_qr_data_buf[index++]=pbuf[i]; 
		}
		print_qr_data_buf[index++]=0x00;
		/******************************************/
	  /************* end****************/
		print_qr_data_buf[index++]=0x1A ; 
		print_qr_data_buf[index++]=0x5D; 
		print_qr_data_buf[index++]=0x00; 
		print_qr_data_buf[index++]=0x1A;
		print_qr_data_buf[index++]=0x4F; 
		print_qr_data_buf[index++]=0x00; 
		
    HAL_UART_Transmit(&huart3, (uint8_t*)print_qr_data_buf,index, 0xffff); 
}
void  u3_unpack(uint8_t *state)
{
	 if(UsartType3.RX_flag)
	 {
		     UsartType3.RX_flag=0;
		      *state=1;
	 }
	 else
	 {
		     *state=0;
	 }
	
}