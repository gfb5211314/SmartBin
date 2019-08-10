#include  "bsp_door.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"




door_variate_type door_variate;

/*********开始计数*********/
void open_door()
{
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_15, GPIO_PIN_SET);
}
void close_door()
{
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_15, GPIO_PIN_RESET);
}
void stop_door()
{
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
}

void door_tim()
{
	   if(door_variate.start_flag==1)
		 {
	      door_variate.tim_count++;
	    if(door_variate.tim_count>20000)
	    {
		    door_variate.start_flag=0;
				door_variate.timout_flag=1;
	    }
		}
		 else
		 {
			 door_variate.start_flag=0;
			 door_variate.tim_count=0;
			 
		 }
}

void set_tim_flag(uint8_t sta)
{
	door_variate.start_flag=sta;
	
}
void get_timout_flag(uint8_t *sta)
{
	 *sta=door_variate.timout_flag;
	
}
void set_timout_flag(uint8_t sta)
{
	 door_variate.timout_flag=sta;
	
}
