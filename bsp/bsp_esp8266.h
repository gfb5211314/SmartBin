#ifndef   __BSP_ESP8266_H__
#define   __BSP_ESP8266_H__


#include "stm32f4xx_hal.h"

 typedef enum
{  
	  set_mode_state=0,
   connect_wifi_state=1,
  connect_web_state=2,
	set_ospf_state=3,
	send_data_state=4,
	zhuce_start=5,
	zhuce_comple
} esp8266_state_type;
 typedef struct{
	 uint8_t   set_mode_state_flag;
	 uint8_t   connect_wifi_state_flag;
   uint8_t  connect_web_state_flag;
   uint8_t	set_ospf_state_flag;
	 uint8_t send_data_state_flag;
   uint8_t	zhuce_start_flag;
   uint8_t	zhuce_comple_flag; 
 }esp8266_state_flag_type;
void send_string(uint8_t *p);

typedef char * (*CallbackTemplate)(char *p,...);
 
uint8_t esp8266_rec_data(uint8_t Receive_data_flag,uint8_t *rec_buf);
void send_byte(uint8_t p);
void esp_8266_connect_web(esp8266_state_type esp8266_state ,uint8_t Receive_data_flag,uint8_t *rec_buf);
void get_data_web_post( double weight,uint8_t category,char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function);

#endif
