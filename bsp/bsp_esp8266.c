#include "bsp_esp8266.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "md5.h"
extern UART_HandleTypeDef huart7;
const uint8_t set_mode_command[]="AT+CWMODE=3\r\n";
const uint8_t connect_wifi_command[]="AT+CWJAP=\"NetGear-2.4G\",\"yijianlalala\"\r\n";
const uint8_t connect_web_socket[] ="AT+CIPSTART=\"TCP\",\"47.98.164.115\",80\r\n";
const uint8_t set_ospf_command[]= "AT+CIPMODE=1\r\n";
const uint8_t send_data_command[]= "AT+CIPSEND\r\n";
const uint8_t close_ospf_command[]= "+++";
const uint8_t time_web_tcp[]="AT+CIPSTART=\"TCP\",\"api.k780.com\",88\r\n";
const uint8_t RST_COMMAND[]="AT+RST\r\n";

/**
 * @brief esp_8266_connect_web
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */

esp8266_state_flag_type esp8266_state_flag;
esp8266_state_type esp8266_statu;
uint8_t test_local_flag;
//uint8_t  tmer[]
/**
 * @brief  esp_8266_connect_web   注册ESP模块到服务器
 * @param esp8266_state
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */

void esp_8266_connect_web(esp8266_state_type esp8266_state,uint8_t Receive_data_flag,uint8_t *rec_buf)
{
    //one SET mode
//  	char* p;
//	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s","openapi.baidu.com","80");
    if(zhuce_start==esp8266_state)
    {

        esp8266_statu=set_mode_state;
    }
    switch (esp8266_statu)
    {
    case set_mode_state :
        if(esp8266_state_flag.set_mode_state_flag==0)
        {

            esp8266_state_flag.set_mode_state_flag=1;
            send_string((uint8_t *)set_mode_command);
        }
        else
        {
            if(esp8266_rec_data(Receive_data_flag,rec_buf))
            {

                esp8266_state_flag.set_mode_state_flag=0;
                esp8266_statu=connect_wifi_state;
            }
        }

        break;
    case connect_wifi_state :

        if(esp8266_state_flag.connect_wifi_state_flag==0)
        {
            esp8266_state_flag.connect_wifi_state_flag=1;
            send_string((uint8_t *)connect_wifi_command);
        }
        else
        {
            if(esp8266_rec_data(Receive_data_flag,rec_buf))
            {

                esp8266_state_flag.connect_wifi_state_flag=0;
                esp8266_statu=connect_web_state;
            }
        }

        break;
    case connect_web_state :
        if(esp8266_state_flag.connect_web_state_flag==0)
        {
            esp8266_state_flag.connect_web_state_flag=1;
            send_string((uint8_t *)connect_web_socket);
        }
        else
        {
            if(esp8266_rec_data(Receive_data_flag,rec_buf))
            {
                esp8266_state_flag.connect_web_state_flag=0;
                esp8266_statu=set_ospf_state;
            }
        }
        break;
    case set_ospf_state :
        if(esp8266_state_flag.set_ospf_state_flag==0)
        {
            esp8266_state_flag.set_ospf_state_flag=1;
            send_string((uint8_t *)set_ospf_command);
        }
        else
        {
            if(esp8266_rec_data(Receive_data_flag,rec_buf))
            {
                esp8266_state_flag.set_ospf_state_flag=0;
                esp8266_statu=send_data_state;
            }
        }
        break;
    case send_data_state :
        if(esp8266_state_flag.send_data_state_flag==0)
        {

            esp8266_state_flag.send_data_state_flag=1;
            send_string((uint8_t *)send_data_command);
        }
        else
        {
            if(esp8266_rec_data(Receive_data_flag,rec_buf))
            {

                esp8266_state_flag.send_data_state_flag=0;
                esp8266_statu=zhuce_comple;
                test_local_flag=1;
            }
        }
        break;
    default:
        break;
    }
}

uint8_t esp8266_rec_data(uint8_t Receive_data_flag,uint8_t *rec_buf)
{

    if(Receive_data_flag)
    {

        if(strstr((const char*)rec_buf,(const char*)"OK"))
        {
            if(esp8266_state_flag.set_mode_state_flag)
            {

                printf("SET MODE SUCESS\r\n");
                esp8266_state_flag.set_mode_state_flag=0;
                return 1;
            }
            if(esp8266_state_flag.connect_wifi_state_flag)
            {

                printf("connect_wifi_state SUCESS\r\n");
                esp8266_state_flag.connect_wifi_state_flag=0;
                return 1;
            }

            if(esp8266_state_flag.connect_web_state_flag)
            {

                printf("connect_web_state SUCESS\r\n");
                esp8266_state_flag.connect_web_state_flag=0;
                return 1;
            }



            if(esp8266_state_flag.set_ospf_state_flag)
            {

                printf("set_ospf_state SUCESS\r\n");
                esp8266_state_flag.set_ospf_state_flag=0;
                return 1;
            }
            if(esp8266_state_flag.send_data_state_flag)
            {
                printf("%s\r\n",rec_buf);
                printf("send_data_state SUCESS\r\n");
                esp8266_state_flag.send_data_state_flag=0;
                return 1;
            }
        }

        Receive_data_flag=0;
    }


}
void send_byte(uint8_t p)
{

    HAL_UART_Transmit(&huart2, (uint8_t*)&p,1, 0xffff);

}
//send string
void send_string(uint8_t *p) {

    while(*p!= '\0') {

        send_byte(*p);
        p++;
    }
}
void test()
{
//	send_string((uint8_t *)connect_web_socket);
//	printf("%d", strlen(connect_web_socket));
}
void get_beijing_tim()
{
    // 时间API
 // http://api.k780.com:88/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json

//	 char tim_api[200];
//	 char tim_tem[50];
//  strcpy ((char*)tim_api,"GET /?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json HTTP/1.1\r\n"); //post
//	strcat (tim_api,"Host:  api.k780.com:88\r\n");   //host
//	strcat (tim_api,"\r\n\r\n"); //
//   send_string( (uint8_t *)tim_api);
//	printf("%s\r\n",tim_api);
}
/**
* @brief 
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */

void get_data_web_post( double weight,uint8_t category,char *appid, char *appkey,char *account,char *timestamp,CallbackTemplate md5function)
{
    char p[400];
    char p1[50];
    //jisuan changdu
    char p2[200];
    char p3[20];
    char *encrypt_data;
    uint16_t data_len;
    encrypt_data=md5function(appid, appkey,account,timestamp);
    printf("%s",(char*)encrypt_data);
    sprintf (p2,"app_id=%s&account=%s&encrypt_data=%s&timestamp=%s&type=%d&weight=%.2lf",appid,account,encrypt_data,timestamp,category,weight); //
//	strcpy (p2,"app_id=104501498&account=131010401280001853&encrypt_data=32c5be0c19b705603c72f6bc2e52bddb&timestamp=1564073400"); //
//	sprintf(p3,"&type=%d",category);//垃圾类型
//	strcat (p2,p3); //
//	sprintf(p3,"&weight=%.2lf",weight);//垃圾重量
//	strcat (p2,p3);
    data_len=strlen((const char *)p2);
    //one
//	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s","openapi.baidu.com","80");
//	http://room.yijingmedia.com/open/push 。
    strcpy ((char*)p,"POST /open/push HTTP/1.1\r\n"); //post
    strcat (p,"Host: room.yijingmedia.com\r\n");   //host
    strcat (p,"Content-Type: application/x-www-form-urlencoded\r\n"); //
    sprintf(p1,"Content-Length: %d\r\n",data_len);
    strcat (p,p1); //
    strcat (p,"\r\n"); //
//	strcat (p,"app_id=104501498&account=131010401280001853&encrypt_data=32c5be0c19b705603c72f6bc2e52bddb&timestamp=1564073400"); //
//	sprintf(p1,"&type=%d",category);//垃圾类型
//	strcat (p,p1); //
//	sprintf(p1,"&weight=%.2lf",weight);//垃圾重量
//	strcat (p,p1);
    strcat (p,p2);
    send_string( (uint8_t *)p);
    printf("%s\r\n",p);
}
void poll_data_web(uint8_t Receive_data_flag,uint8_t *rec_buf,uint8_t *states)
{
	 
	if(Receive_data_flag)
	{


     if(strstr((const char*)rec_buf,(const char*)"Success"))
        {
            *states=1;   
				}
				else{
					
					 *states=0;
				}
		Receive_data_flag=0;
	} 
	
}
