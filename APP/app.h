#ifndef __APP_H_
#define __APP_H_
#include "stm32f4xx_hal.h"
#include "common_usart.h"
typedef enum
{
   Qr_code_idle = 0,
  Qr_code_busy
} Qr_code_state_type;

typedef enum
{
  system_idle = 0,   //开启FRID和扫码
	system_busy = 1,
	system_result = 2
//  Qr_code_busy
}Smart_bin_system_state_type;

typedef struct {
    /***系统全部状态*************/
    uint8_t Card_Check;     //卡片校验状态
    uint8_t Qr_code_check;  //二维码校验状态
    uint8_t print_status;  //打印机完成状态
    uint8_t print_paper;       //容量状态
    uint8_t Mechanical_key;  //机械按键状态
    uint8_t Touch_key;       //触摸屏按键状态
    /*******系统调用数据**************/
    /*******填充CODE******************/
    uint8_t Qr_code_data[100]; //二维码数据
    uint16_t Qr_code_data_len;
    uint8_t account[100];

    uint8_t Card_data[100];    //卡片数据
    uint16_t weight_data;       //重量
    uint8_t  web_data[100];     //后台数据
    uint8_t   timestamp[100];
    uint32_t   timestamp_rtc;
    /***********************************/
    uint8_t Qr_code_data_flag;    //检测是否有数据
    uint8_t web_data_flag;    //检测是否有数据
    uint8_t door_flag;    //检测men
    uint8_t height_flag;    //检测men
    uint8_t print_state_flag;
    uint8_t tim_count_flag;
    uint32_t weight_last;
    uint32_t weight_end;
    uint8_t door_timout_flag;
    uint8_t api_get_tim_flag;
    uint8_t api_get_info_flag;
    uint8_t user_msg_flag;
    uint8_t door_open_tim_flag;
    uint16_t door_open_tim_count;
    uint8_t *appid;
    uint8_t *appkey;
    uint8_t type;
    double height;
    uint8_t handle_time[30];
    uint8_t *device_id;
    uint8_t *door_no;
    uint8_t get_tmie_sucess;
    uint8_t open_push_sucess;
		uint32_t weight_ad_last;
		uint32_t weight_ad_end;
		uint32_t weight_last_tim_count;
		uint32_t weight_end_tim_count;
		uint32_t weight_tim_out;
		uint32_t weight_over;
    /*************函数*********************/

} Bin_System_Type;
void GSM_HTTP_INIT();
 



void System_init_param_init();

void timer_update();

void System_ilde_task();

void resend_task();

void poll_GSM_rec_data(USART_RECEIVETYPE *usart);



void usart6_weight_rec_data(USART_RECEIVETYPE *usartxx,uint32_t *state,uint32_t *ad_data);

void all_usart_rec_data();

void GSM_HTTP_INIT();


/*****************
     test
****************/
void inity();










#endif
