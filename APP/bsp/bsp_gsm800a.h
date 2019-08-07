#ifndef  __BSP_GSM800A_H__
#define  __BSP_GSM800A_H__

#include "stm32f4xx.h"

typedef struct{
	 uint8_t timestamp[10];
	 uint8_t * weather_api_path;
   uint8_t  weather_api_param[300];
	 uint8_t * getpoint_api_path;
   uint8_t  getpoint_api_param[300];
	 uint8_t * get_info_api_path;
	 uint8_t  get_info_api_param[300];
	 uint8_t * open_push_api_path;
	 uint8_t open_push_api_param[300];
	 uint8_t * gsm_context_type;
	 uint8_t *  post_get;
	  uint16_t ALT_year;
    uint8_t  ALT_month;
    uint8_t  ALT_day;
    uint8_t  ALT_hour;
    uint8_t  ALT_minute;
    uint8_t  ALT_second;
}gsm_param_type; 
typedef char * (*CallbackTemplate)(char *p,...);
//uint8_t  gsm_weather_com[]="AT+HTTPPARA=\"URL\",\"api.k780.com:88/\"\r\n";        //天气API
//uint8_t  gsm_weather_api_param[]="app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json";        //天气参数
//uint8_t  gsm_weijing_get_point[]="AT+HTTPPARA=\"URL\",\"room.yijingmedia.com/open/get_points\"\r\n";  //积分
//uint8_t  gsm_weijing_get_point_param[]="app_id=104501498&account=131010401280001853&encrypt_data=32c5be0c19b705603c72f6bc2e52bddb&timestamp=1564073400";        //积分
//uint8_t  gsm_context_type[]="AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n"; //文本类型

uint8_t gsm_post_get_time(uint8_t *Receive_data_flag,uint8_t *rec_buf);
uint8_t poll_rec_data(uint8_t *Receive_data_flag,uint8_t *rec_buf);
uint8_t gsm_post_getinfo(uint8_t *Receive_data_flag, uint8_t *rec_buf,char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function);
#endif