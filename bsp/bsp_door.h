#ifndef  __BSP_DOOR_H__
#define  __BSP_DOOR_H__

#include "stm32f4xx_hal.h"
typedef struct{
	  uint16_t tim_count;
	  uint8_t start_flag;
	 uint8_t  timout_flag;
	  uint16_t tim_count_com; //ji shu wan cheng
	
}door_variate_type;


void open_door(void);
void close_door(void);




void open_door(void);

void close_door(void);

void stop_door(void);


void door_tim(void);


void set_tim_flag(uint8_t sta);
void get_timout_flag(uint8_t *sta);

void set_timout_flag(uint8_t sta);



#endif