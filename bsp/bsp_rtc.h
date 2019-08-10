#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include "stm32f4xx_hal.h"



typedef struct{
	  uint16_t year;
	
	
	
	
	
	
}haha;



uint32_t get_timer(void);


void set_time_data(uint16_t Year,uint16_t Month,uint16_t Date,uint16_t Hours, uint16_t Minutes ,uint16_t Seconds);


uint32_t time2Stamp(RTC_DateTypeDef date, RTC_TimeTypeDef time);    //北京时间转时间戳



 /**
  * @brief  Get the current time 
  * @param  void
  * @param  void
  * @return void
  * @retval void
  */
uint32_t get_timer();

/**
 * @brief  set time
 * @param Year()
 * @param 形参 参数说明
 * @return void
 * @retval void
 */
void set_time_data(uint16_t Year,uint16_t Month,uint16_t Date,uint16_t Hours, uint16_t Minutes ,uint16_t Seconds);



#endif

