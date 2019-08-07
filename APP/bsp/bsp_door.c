#include  "bsp_door.h"
#include "stm32f4xx_hal.h"


typedef struct{
	  uint16_t tim_count;
	  uint8_t start_flag;
	 uint8_t  timout_flag;
	  uint16_t tim_count_com; //ji shu wan cheng
	
}door_variate_type;

door_variate_type door_variate;

/*********��ʼ����*********/
void open_door()
{
	
}
void close_door()
{
	
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