#include "bsp_gsm800a.h"
#include "md5.h"
#include "stm32f4xx.h"
#include "bsp_esp8266.h"
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "bsp_print.h"
#include "bsp_door.h"
//  AT+HTTPPARA="URL","api.k780.com:88/"
#define   GSM_USART           &huart3
#define Wait_time   osDelay(100);

//驱动指令

uint8_t AT_SAPBR_1[]="AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n";
uint8_t AT_SAPBR_2[]="AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n";
uint8_t AT_SAPBR_3[]="AT+SAPBR=1,1\r\n";  //打开
uint8_t AT_SAPBR_4[]="AT+SAPBR=0,1\r\n"; //关闭
uint8_t AT_HTTPINIT[]="AT+HTTPINIT\r\n";
uint8_t AT_HTTPPARA[]="AT+HTTPPARA=\"CID\",1\r\n";
//关掉HTTP
uint8_t AT_HTTPTERM[]="AT+HTTPTERM\r\n";


// 查询指令
uint8_t   AT_CREG[]="AT+CREG?\r\n"; //查看GSM是否注册成功
uint8_t   AT_CGREG[]="AT+CGREG?\r\n"; //查看GPRS是否注册成功
uint8_t   AT_CGATT[]="AT+CGATT?\r\n"; //查看GPRS附着状态


//获取模板时钟

uint8_t AT_CNTP[]="AT+CNTP=\"ntp1.aliyun.com\",32\r\n";
uint8_t AT_CNTP_1[]="AT+CNTP\r\n";
uint8_t AT_CCLK[]="AT+CCLK?\r\n";

////业务指令
//uint8_t  gsm_weather_com[]="AT+HTTPPARA=\"URL\",\"api.k780.com:88/\"\r\n";        //天气API
//uint8_t  gsm_weather_api_param[]="app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json";        //天气参数
//uint8_t  gsm_weijing_get_point[]="AT+HTTPPARA=\"URL\",\"room.yijingmedia.com/open/get_points\"\r\n";  //积分
//uint8_t  gsm_weijing_get_point_param[]="app_id=104501498&account=131010401280001853&encrypt_data=32c5be0c19b705603c72f6bc2e52bddb&timestamp=1564073400";        //积分
//uint8_t  gsm_context_type[]="AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n"; //文本类型
//uint8_t  gsm_post_get[]="AT+HTTPACTION=1\r\n";  //
//uint8_t gsm_AT_HTTPREAD[]="AT+HTTPREAD\r\n";//读数据

extern UART_HandleTypeDef huart4;
uint8_t sendcomd_flag=0;
USART_RECEIVETYPE  UsartType4;
gsm_param_type gsm_param;
/*Define print function*/


void DMA_USART4_IDLE_INIT()
{

//	DMA_START(&UsartType2,&huart2);
    DMA_START(&UsartType4,&huart4);


}


void UART4_IRQHandler(void)
{
    /* USER CODE BEGIN UART4_IRQn 0 */
    UsartReceive_IDLE(&huart4,&UsartType4);
    /* USER CODE END UART4_IRQn 0 */
    HAL_UART_IRQHandler(&huart4);
    /* USER CODE BEGIN UART4_IRQn 1 */

    /* USER CODE END UART4_IRQn 1 */
}

void gsm_usart_send_byte(uint8_t p)
{

    HAL_UART_Transmit(GSM_USART, (uint8_t*)&p,1, 0xffff);

}
//send string
void gsm_send_string(uint8_t *p) {

    while(*p!= '\0') {

        gsm_usart_send_byte(*p);
        p++;
    }
}
/**
  * @brief  Program halfword, word or double word at a specified address
  * @note   The function HAL_FLASH_Unlock() should be called before to unlock the FLASH interface
  *         The function HAL_FLASH_Lock() should be called after to lock the FLASH interface
  *
  * @note   If an erase and a program operations are requested simultaneously,
  *         the erase operation is performed before the program one.
  *
  * @note   FLASH should be previously erased before new programmation (only exception to this
  *         is when 0x0000 is programmed)
  *
  * @param  TypeProgram:  Indicate the way to program at a specified address.
  *                       This parameter can be a value of @ref FLASH_Type_Program
  * @param  Receive_data_flag:      Specifies the address to be programmed.
  * @param  rec_buf:         Specifies the data to be programmed
  *
  * @retval uint8_t      1 sucess  0 fail
  */
extern USART_RECEIVETYPE  UsartType3;
uint8_t send_command(uint8_t *command,uint8_t *wait_sty,uint8_t wait_time,USART_RECEIVETYPE *usart)
{

    uint8_t  wait_tim_count=0;
    uint8_t  send_state;
    uint8_t  ste=0;
    char *timePtr = NULL;
    do {
        gsm_send_string( (uint8_t *)command);
        sendcomd_flag=1;
        wait_tim_count++;

        Wait_time;
        if(usart->RX_flag)
        {
//	      printf("%s",    usart->RX_pData );
//					  printf("wait_tim_count=%d",wait_tim_count);
            if(strstr((const char*)usart->RX_pData,(const char*)wait_sty))
            {

                if(strstr((const char*)usart->RX_pData,(const char*)"CCLK:"))
                {
//							printf("rec_buf=%s\r\n",rec_buf);
                    timePtr = strstr((const char*)usart->RX_pData,(const char*)"CCLK:");
                    gsm_param.ALT_year    = 10*(timePtr[7] - '0') + timePtr[8] - '0';
                    gsm_param.ALT_month   = 10*(timePtr[10] - '0') + timePtr[11] - '0';
                    gsm_param.ALT_day     = 10*(timePtr[13] - '0') + timePtr[14] - '0';
                    gsm_param.ALT_hour    = 10*(timePtr[16] - '0') + timePtr[17] - '0';
                    gsm_param.ALT_minute  = 10*(timePtr[19] - '0') + timePtr[20] - '0';
                    gsm_param.ALT_second  = 10*(timePtr[22] - '0') + timePtr[23] - '0';
//							printf("  gsm_param.ALT_year=%d\r\n" , gsm_param.ALT_year);
//							printf("  gsm_param.ALT_month=%d\r\n" , gsm_param.ALT_month);
//							printf("  gsm_param.ALT_day=%d\r\n" , gsm_param.ALT_day);
//							printf("  gsm_param.ALT_hour=%d\r\n" , gsm_param.ALT_hour);
//							printf("  gsm_param.ALT_minute=%d\r\n" , gsm_param.ALT_minute);
//							printf("  gsm_param.ALT_second=%d\r\n" , gsm_param.ALT_second);
                }
                printf("set %s sucess\r\n",command);
                 usart->RX_flag=0;
                  send_state=1;
                   ste=1;
                 sendcomd_flag=2;

            }
            else
            {
                if(wait_tim_count>wait_time)
                {
                    wait_tim_count=0;
                    //跳出循环
                     usart->RX_flag=0;
                    send_state=0;
                    ste=1;
                    printf("set %s 1fail\r\n",command);
                }
            }
          memset(usart->RX_pData, 0, usart->RX_Size);
        }
        else
        {
            //time out
            if(wait_tim_count>wait_time)
            {
                wait_tim_count=0;
                //跳出循环
                send_state=0;
                ste=1;
                printf("set %s 2fail\r\n",command);
           }

        }
    }
    while(ste==0);
    return send_state;
}
 void read_gsm_data(uint8_t *datalen)
 {
	    uint8_t  gsm_AT_HTTPREAD[50];
	   memset(gsm_AT_HTTPREAD, 0,sizeof(gsm_AT_HTTPREAD));
	  sprintf ((char*)gsm_AT_HTTPREAD,"AT+HTTPREAD=1,%s\r\n",datalen); //
//	  printf("gsm_AT_HTTPREAD=%shaha",gsm_AT_HTTPREAD);
	  gsm_send_string( (uint8_t *)gsm_AT_HTTPREAD);
 }
void get_ntp_time(USART_RECEIVETYPE *usart)
{


    send_command(AT_CNTP,(uint8_t *)"OK",200,usart);
    send_command(AT_CNTP_1,(uint8_t *)"+CNTP: 1",200,usart);
    send_command(AT_CCLK,(uint8_t *)"OK",200,usart);

}
void gsm_register(USART_RECEIVETYPE *usart)
{
    //send_command("AT+RST\r\n","OK",5,Receive_data_flag,rec_buf);

//    send_command("AT\r\n",(uint8_t *)"OK",200,usart);
       yellow_led_close();
	     	 red_led_close();
	      green_led_close();
	  send_command("AT+CSQ\r\n",(uint8_t *)"OK",200,usart);
    send_command(AT_CREG,(uint8_t *)"0,1",200,usart);
    send_command(AT_CGREG,(uint8_t *)"0,1",200,usart);
    send_command(AT_CGATT,(uint8_t *)"1",200,usart);
    send_command(AT_HTTPTERM,(uint8_t *)"OK",200,usart);
    send_command(AT_SAPBR_1,(uint8_t *)"OK",200,usart);
    send_command(AT_SAPBR_2,(uint8_t *)"OK",200,usart);
    send_command(AT_SAPBR_3,(uint8_t *)"OK",200,usart);
    get_ntp_time(usart);
    if(send_command(AT_HTTPINIT,(uint8_t *)"OK",200,usart))
		{
			  yellow_led_open();
		  	 red_led_close();
				 yellow_led_close();
		}
		else
		{ 
		    yellow_led_open();
				 red_led_open();
		}
    send_command(AT_HTTPPARA,(uint8_t *)"OK",200,usart);
}

/**
 * @brief  get beijing time
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */

void gsm_param_time_msg_init()
{
    gsm_param.weather_api_path=(uint8_t *)"AT+HTTPPARA=\"URL\",\"api.k780.com:88/\"\r\n";
    strcpy ((char*)gsm_param.weather_api_param,"app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json\r\n"); //post
    gsm_param.gsm_context_type=(uint8_t *)"AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n";
    gsm_param.post_get=(uint8_t *)"AT+HTTPACTION=1\r\n";
//	        printf("gsm_param.weather_api_path=%s",gsm_param.weather_api_path);

}
//     char data_numeb[200];
uint8_t gsm_post_get_time(USART_RECEIVETYPE *usart)
{
    uint8_t state;
    char data_numeb[200];
    gsm_param_time_msg_init();
	   send_command(AT_SAPBR_1,(uint8_t *)"OK",200,usart);
     send_command(AT_SAPBR_2,(uint8_t *)"OK",200,usart);
     send_command(AT_SAPBR_3,(uint8_t *)"OK",200,usart);
	   send_command(AT_HTTPINIT,(uint8_t *)"OK",200,usart);
	   send_command(AT_HTTPPARA,(uint8_t *)"OK",200,usart);
    //first step 发送域名PATH
    if(send_command(gsm_param.weather_api_path,(uint8_t *)"OK",5,usart))
    {

        //two step 发送文本类型
        if( send_command(gsm_param.gsm_context_type,(uint8_t *)"OK",5,usart))
        {
            //three step 发送的参数大小
            sprintf (data_numeb,"AT+HTTPDATA=%d,10000\r\n",strlen((const char *)gsm_param.weather_api_param)); //
//	        printf("data_numeb=%s",data_numeb);
            if( send_command((uint8_t *)data_numeb,(uint8_t *)"OK",5,usart))
                //four step 发送的参数
            {
                if(send_command(gsm_param.weather_api_param,(uint8_t *)"OK",5,usart))
                {   //等待时间要长
                    //five step  采用POST方式发送
                    if(send_command(gsm_param.post_get,(uint8_t *)"OK",5,usart))
                    {
                        state=1;
                        return  state;

                    }
                    else
                    {
                        return  0;
                    }
                }
                else
                {
                    return  0;
                }
            }
            else
            {
                return  0;
            }
        }
        else
        {
            return  0;
        }
    }
    else
    {
        return  0;
    }
    //等待数据返回
}
/**
 * @brief  通过POST 方式发送数据,查询用户信息
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */


void gsm_param_info_msg_init()
{
    gsm_param.get_info_api_path=(uint8_t *)"AT+HTTPPARA=\"URL\",\"room.yijingmedia.com/open/get_info\"\r\n";
    gsm_param.gsm_context_type=(uint8_t *)"AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n";
    gsm_param.post_get=(uint8_t *)"AT+HTTPACTION=1\r\n";
//	        printf("gsm_param.get_info_api_path=%s",gsm_param.get_info_api_path);

}
//char data_numeb[200];
uint8_t gsm_post_getinfo(USART_RECEIVETYPE *usart,char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function)
{
    char *encrypt_data;
    uint16_t data_len;
    char data_numeb[200];
    gsm_param_info_msg_init();
    
    encrypt_data=md5function(appid, appkey,account,timestamp);
//          printf("encrypt_data=%s",(char*)encrypt_data);
    sprintf ((char*)gsm_param.get_info_api_param,"app_id=%s&account=%s&encrypt_data=%s&timestamp=%s",appid,account,encrypt_data,timestamp); //
//          data_len=strlen((const char *)gsm_param.get_info_api_param);
//	printf("gsm_param.get_info_api_param=%s\r\n",gsm_param.get_info_api_param);
//  printf("len=%d\r\n",strlen((const char *)gsm_param.get_info_api_param));
     send_command(AT_SAPBR_1,(uint8_t *)"OK",200,usart);
     send_command(AT_SAPBR_2,(uint8_t *)"OK",200,usart);
     send_command(AT_SAPBR_3,(uint8_t *)"OK",200,usart);
	   send_command(AT_HTTPINIT,(uint8_t *)"OK",200,usart);
	   send_command(AT_HTTPPARA,(uint8_t *)"OK",200,usart);
    //first step 发送域名PATH
    if(send_command(gsm_param.get_info_api_path,(uint8_t *)"OK",5,usart))
    {
        //two step 发送文本类型
        if(send_command(gsm_param.gsm_context_type,(uint8_t *)"OK",5,usart))
        {
            //three step 发送的参数大小命令
            sprintf (data_numeb,"AT+HTTPDATA=%d,10000\r\n",strlen((const char *)gsm_param.get_info_api_param)); //
//	        printf("data_numeb=%s",data_numeb);
            //three step 发送的参数大小命令
            if(send_command((uint8_t *)data_numeb,(uint8_t *)"DOWNLOAD",5,usart))
            {
                //four step 发送的参数
                if(send_command(gsm_param.get_info_api_param,(uint8_t *)"OK",255,usart))
                {   //等待时间要长
                    //five step  采用POST方式发送
                    if(send_command(gsm_param.post_get,(uint8_t *)"OK",5,usart))
                    {

                        return 1;
                    }
                    else
                    {
                        return  0;
                    }
                }
                else
                {
                    return  0;
                }
            }
            else
            {
                return  0;
            }
        }
        else
        {
            return  0;
        }
    }
    else
    {
        return  0;
    }
    //等待数据返回
}
void close_http(USART_RECEIVETYPE *usart)
{
	send_command(AT_HTTPTERM,(uint8_t *)"OK",200,usart);//关闭网络	AT_HTTPTERM
	send_command(AT_SAPBR_4,(uint8_t *)"OK",200,usart);//关闭网络
	
	
}
void gsm_param_push_msg_init()
{
    gsm_param.get_info_api_path=(uint8_t *)"AT+HTTPPARA=\"URL\",\"room.yijingmedia.com/open/push\"\r\n";
    gsm_param.gsm_context_type=(uint8_t *)"AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n";
    gsm_param.post_get=(uint8_t *)"AT+HTTPACTION=1\r\n";
    printf("gsm_param.get_info_api_path=%s",gsm_param.get_info_api_path);

}
uint8_t gsm_post_openpsuh(USART_RECEIVETYPE *usart,
                          char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function,
                          int type,double height,char *handle_time,char *device_id,char *door_no)
{
    char *encrypt_data;
    uint16_t data_len;
    encrypt_data=md5function(appid, appkey,account,timestamp);
    printf("%s",(char*)encrypt_data);
    sprintf ((char*)gsm_param.get_info_api_param,"app_id=%s&account=%s&encrypt_data=%s&timestamp=%s&type=%d&weight=%.2f&handle_time=%s&device_id=%s&door_no=%s",
             appid,account,encrypt_data,timestamp,type,height,handle_time,device_id,door_no); //
//          data_len=strlen((const char *)gsm_param.get_info_api_param);
    char data_numeb[200];
    gsm_param_push_msg_init();
    //first step 发送域名PATH
    if(send_command(gsm_param.get_info_api_path,(uint8_t *)"OK",5,usart))
    {
        //two step 发送文本类型
        if(send_command(gsm_param.gsm_context_type,(uint8_t *)"OK",5,usart))
        {
            //three step 发送的参数大小命令
            sprintf (data_numeb,"AT+HTTPDATA=%d,10000\r\n",strlen((const char *)gsm_param.get_info_api_param)); //
//	        printf("data_numeb=%s",data_numeb);
            //three step 发送的参数大小命令
            if( send_command((uint8_t *)data_numeb,(uint8_t *)"DOWNLOAD",5,usart))
            {
                //four step 发送的参数

                if( send_command(gsm_param.get_info_api_param,(uint8_t *)"OK",50,usart))//等待时间要长
                {
                    //five step  采用POST方式发送
                    if(send_command(gsm_param.post_get,(uint8_t *)"OK",5,usart))
                    {
                        return  1;

                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    //等待数据返回
}
