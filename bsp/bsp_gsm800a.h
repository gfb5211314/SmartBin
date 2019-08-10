#ifndef  __BSP_GSM800A_H__
#define  __BSP_GSM800A_H__

#include "stm32f4xx.h"
#include "common_usart.h"
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

uint8_t gsm_post_get_time(USART_RECEIVETYPE *usart);
uint8_t poll_rec_data(USART_RECEIVETYPE *usart);
uint8_t gsm_post_getinfo(USART_RECEIVETYPE *usart,char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function);
void get_ntp_time(USART_RECEIVETYPE *usart);
void gsm_register(USART_RECEIVETYPE *usart);
void gsm_usart_send_byte(uint8_t p);
 void read_gsm_data(uint8_t *datalen);
//send string
void gsm_send_string(uint8_t *p) ;

uint8_t send_command(uint8_t *command,uint8_t *wait_sty,uint8_t wait_time,USART_RECEIVETYPE *usart);

void get_ntp_time(USART_RECEIVETYPE *usart);

void gsm_register(USART_RECEIVETYPE *usart);


/**
 * @brief  get beijing time
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */

void gsm_param_time_msg_init(void);

//     char data_numeb[200];
uint8_t gsm_post_get_time(USART_RECEIVETYPE *usart);


/**
 * @brief  通过POST 方式发送数据,查询用户信息
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */


void gsm_param_info_msg_init(void);

//char data_numeb[200];
uint8_t gsm_post_getinfo(USART_RECEIVETYPE *usart,char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function);

void gsm_param_push_msg_init(void);

uint8_t gsm_post_openpsuh(USART_RECEIVETYPE *usart,
                          char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function,
                          int type,double height,char *handle_time,char *device_id,char *door_no);

#endif