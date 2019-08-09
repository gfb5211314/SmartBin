#include "bsp_weight.h"
#include "usart.h"
#include "bsp_print.h"
#include "stdio.h"
#include "string.h"
USART_RECEIVETYPE  UsartType6;
/**
 * @brief calibration weight sensor
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */
void  calibration_weight_zero_command()
{
	    uint8_t   index=0;
	    uint8_t   calibration_weight_zero_buf[10]; 
	    calibration_weight_zero_buf[index++]=0xAA;
	    calibration_weight_zero_buf[index++]=0x00;
	    calibration_weight_zero_buf[index++]=0xA9;
	    calibration_weight_zero_buf[index++]=0xAB;
	    calibration_weight_zero_buf[index++]=0xA8;
	 HAL_UART_Transmit(&huart6, (uint8_t*)calibration_weight_zero_buf,index, 0xffff); 
	
}
/**
 * @brief calibration weight sensor
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */
void  get_weight_command()
{
	    uint8_t   index=0;
	    uint8_t   get_weight_data[10]; 
	    get_weight_data[index++]=0xa3;
	    get_weight_data[index++]=0;
	    get_weight_data[index++]=0xA2;
	    get_weight_data[index++]=0xA4;
	    get_weight_data[index++]=0xA5;
	 HAL_UART_Transmit(&huart6, (uint8_t*)get_weight_data,index, 0xffff); 
}
void get_ad_weight_command()
{
	   uint8_t   index=0;
	    uint8_t   get_weight_data[10]; 
	    get_weight_data[index++]=0xA1;
	    get_weight_data[index++]=0;
	    get_weight_data[index++]=0xA0;
	    get_weight_data[index++]=0xA2;
	    get_weight_data[index++]=0xA3;
	 HAL_UART_Transmit(&huart6, (uint8_t*)get_weight_data,index, 0xffff); 
}




void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

UsartReceive_IDLE(&huart6,&UsartType6);
  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}
void DMA_USART6_IDLE_INIT()
{

    DMA_START(&UsartType6,&huart6);

}
uint8_t  u6_unpack()  //不能定义 uint8_t state  ，否则传不了参数
{
       uint8_t	state;
	 if(UsartType6.RX_flag)
	 {
	
		  UsartType6.RX_flag=0;
		     state=1;
		 return state;
	 }
	 	 else
	 {
         state=0; 
		 return state;
	 }
	
}
/***get  g**************/
void  get_weight_value(uint16_t * state)
{
	   if((UsartType6.RX_pData[0]==0xAA)&&(UsartType6.RX_pData[9]==0xff))
		 {
			    *state = (UsartType6.RX_pData[4]*65536+ UsartType6.RX_pData[5]*256+ UsartType6.RX_pData[6])*10;//g
			 
		 }
	   
}


