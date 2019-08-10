
/**
 * @file   app.c
 * @brief  垃圾桶业务逻辑框架
 * @details 细节
 * @mainpage 工程概览
 * @author Fubiao.Guo
 * @email  2545188523@qq.com
 * @version V1.0
 * @date 2019/7/19
 * @license 深圳市琴弦科技有限公司
 */

#include "app.h"
#include "stm32f4xx_hal.h"
#include "api_interface.h"
#include "string.h"
#include "bsp_rc522.h"
#include "bsp_scaner.h"
#include "bsp_gsm800a.h"
#include "common_usart.h"
#include "bsp_print.h"
#include "md5.h"
#include  "bsp_door.h"
#include  "stdio.h"
#include "stdio.h"
#include "string.h"
#include "rtc.h"
#include "cmsis_os.h"
#include "task.h"
#include "FreeRTOS.h"
#include "bsp_led.h"
#include "bsp_weight.h"
#include "bsp_rtc.h"
#define  open_over_weight  0    //0 代表超重，业务正常执行   1 超重，业务不执行 
//#define   open_rtc   0


typedef struct {
    uint8_t	 get_card_qr_flag;
    uint8_t	 get_time_flag;
    uint8_t   get_info_flag;
    uint8_t   get_last_height_flag;
    uint8_t   open_door_flag;
    uint8_t   start_time_flag;
    uint8_t   open_push_flag;
	  uint8_t   overweight_flag;

} Bin_System_task_Type;
Bin_System_task_Type  Bin_System_task;
extern USART_RECEIVETYPE  UsartType3;
extern USART_RECEIVETYPE  UsartType6;
extern USART_RECEIVETYPE  UsartType2;
extern RTC_TimeTypeDef control_time;
extern RTC_DateTypeDef control_date;
//采用一切皆数据和状态模式去对接API
Bin_System_Type  Bin_System;
Qr_code_state_type  Qr_code_state;
extern gsm_param_type gsm_param;
//Smart_bin_system_state_type Smart_bin_system_state;
//Bin_Check_Type  Bin_Check;

//Bin_button_Type  Bin_button; char appid[]="104501498";
char appkey[]="test1234567890";
char account[]="131010401280001853";
char timestamp[]="1564073400";
//Bin_printf_Type  Bin_printf;
void System_init_param_init()
{
    Bin_System.appid=(uint8_t*)"104501498";
    Bin_System.appkey=(uint8_t*)"test1234567890";
//	Bin_System.handle_time=(uint8_t *)"2019-08-01 13:05:30";
    sprintf ((char*)Bin_System.handle_time,"20%d-%02d-%02d %02d:%02d:%02d",gsm_param.ALT_year,gsm_param.ALT_month,
             gsm_param.ALT_day,gsm_param.ALT_hour,gsm_param.ALT_minute,gsm_param.ALT_second);
    printf("Bin_System.handle_time=%s\r\n",Bin_System.handle_time);
    set_time_data(gsm_param.ALT_year,gsm_param.ALT_month,
                  gsm_param.ALT_day,gsm_param.ALT_hour,gsm_param.ALT_minute,gsm_param.ALT_second);
    Bin_System.device_id=(uint8_t *)"test";
    Bin_System.door_no=(uint8_t *)"3";
}
void timer_update()
{


    sprintf ((char*)Bin_System.handle_time,"20%d-%02d-%02d %02d:%02d:%02d",control_date.Year, control_date.Month,
             control_date.Date, control_time.Hours, control_time.Minutes, control_time.Seconds);
    sprintf(Bin_System.timestamp,"%d",get_timer());
    printf("Bin_System.handle_time=%s\r\n",Bin_System.handle_time);
    printf("Bin_System.timestamp=%s\r\n",Bin_System.timestamp);
}

	
void System_ilde_task()
{    

    	
	   if(Bin_System_task.get_card_qr_flag==1)
		 {
			 UsartType2.RX_flag=0;
		 }
    //启动开始任务
    if(Bin_System_task.get_card_qr_flag==0)
    {
        if((rc522_find_card(Bin_System.account)==1)||(u2_unpack(Bin_System.account)==1))
        {
            printf("Bin_System.account=%s\r\n",Bin_System.account);
            Bin_System_task.get_card_qr_flag=1;
            Bin_System_task.get_time_flag=1;
            Bin_System_task.get_info_flag=1;
            Bin_System_task.get_last_height_flag=1;
            printf("获得用户数据\r\n");
					 Bin_System.weight_last=0;
					 Bin_System.weight_end=0;
					  Bin_System.weight_last_tim_count=0;
					  Bin_System.weight_end_tim_count=0;
					    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_SET);
						   osDelay(100);
					   HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_RESET);
					     osDelay(100);
					    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_SET);
					   osDelay(100);
						  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_RESET);
					 
        }
        else
        {
    					   get_timer();
            memset(Bin_System.account, 0, sizeof(Bin_System.account));
           	get_weight_command();
					  osDelay(50);
					  usart6_weight_rec_data(&UsartType6,&Bin_System.weight_over,&Bin_System.weight_ad_last);
					   if((Bin_System.weight_over/1000)>60)
						 {
							 Bin_System_task.overweight_flag=1;
							 
						 }
						 else
						 {
							  Bin_System_task.overweight_flag=0;
							 
						 }
					
        }
    }
	if(	Bin_System_task.overweight_flag==1)
	{
		 HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_SET);
						   osDelay(100);
					   HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_RESET);
					     osDelay(100);
					    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_SET);
					   osDelay(100);
						  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_RESET);
		
	}
    //获取时间任务完成
   if(Bin_System_task.get_time_flag==1)
    {

#ifdef  open_rtc
        //主机发送成功
        if(gsm_post_get_time(&UsartType3.RX_flag,UsartType3.RX_pData))
        {
            //等待数据回应
            printf("发送时间API\r\n");
            Bin_System_task.get_time_flag=2;
        }
#else


        timer_update();
        Bin_System_task.get_time_flag=3;
#endif
    }

    //获取用户时间成功和用户信息
    else if((Bin_System_task.get_info_flag==1)&&(Bin_System_task.get_time_flag==3))
    {
#ifdef  open_rtc
        if(gsm_post_getinfo(&UsartType3.RX_flag,UsartType3.RX_pData,(char *)Bin_System.appid,(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data))
        {
            //等待数据回应
            printf("send user jiaonian \r\n");
            Bin_System_task.get_info_flag=2;
        }
#else
        if(gsm_post_getinfo(&UsartType3,(char *)Bin_System.appid,(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data))
        {
            //等待数据回应
            printf("send user jiaonian \r\n");
            Bin_System_task.get_info_flag=2;
        }
#endif
    }

    //获取称重任务完成
    else if(Bin_System_task.get_last_height_flag==1)
    {
       // 
			
      			get_weight_command();
			          osDelay(30);
			     Bin_System.weight_last_tim_count++;
			     if( Bin_System.weight_last_tim_count>100)
					 {
			 printf("Bin_System.weight_last=%d\r\n",(Bin_System.weight_last));
						 //大于60KG 
						 if((Bin_System.weight_last/1000)>60)
						 {
							Bin_System_task.overweight_flag=0; 			 
						 }
						   if(open_over_weight)
							 {
			     	
							 }
							 else
							 {
								   Bin_System.weight_last_tim_count=0;
					        Bin_System_task.get_last_height_flag=2;	
							 }
					 }
				

    }
	
    //用户正确
   if((Bin_System_task.get_info_flag==3)&&(Bin_System_task.get_last_height_flag==2))
    {
        printf("user is sucess\r\n");
        Bin_System_task.get_info_flag=0;
        Bin_System_task.open_door_flag=1;

    }
    //用户无效
     else  if(Bin_System_task.get_info_flag==4)
    {
        Bin_System_task.get_card_qr_flag=0;
        Bin_System_task.get_info_flag=0;
        Bin_System_task.get_last_height_flag=0;
        Bin_System_task.start_time_flag=0;
        Bin_System_task.open_door_flag=0;
        Bin_System_task.get_time_flag=0;
        Bin_System_task.open_push_flag=0;
//			printf("Bin_System_task.get_info_flag=%d",Bin_System_task.get_info_flag);
    }
    //门打开
   else if(Bin_System_task.open_door_flag==1)
    {
        printf("door is open");
        Bin_System_task.open_door_flag=0;
        open_door();
			 osDelay(12000);
			   stop_door();
       osDelay(15000);
        close_door();
			 osDelay(15000);
        stop_door();
			  Bin_System_task.start_time_flag=1;
    }
    else  if(Bin_System_task.start_time_flag==1)
    {
         		get_weight_command();
			          osDelay(10);
			     Bin_System.weight_end_tim_count++;
			    if( Bin_System.weight_end_tim_count>500)
					{
					    	Bin_System.weight_end_tim_count=0;
							if((Bin_System.weight_end-Bin_System.weight_last)>30)
							{
						
								Bin_System.height=Bin_System.weight_end+Bin_System.weight_last;
							  Bin_System.height=Bin_System.height/1000; //G转为KG
                printf("已经丢垃圾\r\n");
                Bin_System_task.start_time_flag=0;
                Bin_System_task.open_push_flag=1;
						  	Bin_System.weight_end=0;
							  Bin_System.weight_last=0;
							}
							else
							{
								   printf("用户没丢垃圾\r\n");
								  Bin_System_task.get_card_qr_flag=0;
								   Bin_System_task.start_time_flag=0;
									close_http(&UsartType3);
								
							}
				 }
     }
    else if(Bin_System_task.open_push_flag==1)
    {
        printf("推送垃圾信息\r\n");
#ifdef  open_rtc


#else
        timer_update();
#endif
        if(gsm_post_openpsuh(&UsartType3,(char *)Bin_System.appid,(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data,
                             1,Bin_System.height,Bin_System.handle_time,Bin_System.device_id,Bin_System.door_no))
        {
            //等待数据回应
            Bin_System_task.open_push_flag=2;
        }

    }
    //流程完成
    else if(Bin_System_task.open_push_flag==3)
    {
        printf("liuchengwancheng\r\n");
        memset(Bin_System.account, 0, sizeof(Bin_System.account));
        Bin_System_task.open_push_flag=0;
        Bin_System_task.get_card_qr_flag=0;
					Bin_System.weight_end=0;
			  	Bin_System.weight_last=0;
			
    }
    
}
uint16_t test1_cont=0;
uint16_t test2_cont=0;
uint16_t test3_cont=0;

uint16_t test1_recont=0;
uint16_t test2_recont=0;
uint16_t test3_recont=0;

void  hefg()
{
	 
}
//任务重发 5秒重发
void resend_task()
{
		

	
    if(Bin_System_task.get_info_flag==2)
    {
        test1_cont++;
        if(test1_cont>5000)
        {
					test1_recont++;
					 if(test1_recont>5)
					 {
				    	test1_recont=0;
               Bin_System_task.get_card_qr_flag=0;
					    Bin_System.weight_end=0;
							Bin_System.weight_last=0;
						 Bin_System_task.get_info_flag=0;
						  	 red_led_open();
						     yellow_led_open();
						     green_led_close();
						   	 
						 
					 }
            test1_cont=0;
            printf("5miao send user jiaonian \r\n");
            GSM_HTTP_INIT();
            gsm_post_getinfo(&UsartType3,(char *)Bin_System.appid,
                             (char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data);
        }
    }
    else
    {
			test1_recont=0;
        test1_cont=0;

    }
    if(Bin_System_task.get_time_flag==2)
    {
        test2_cont++;
        if(test2_cont>5000)
        {
            test2_cont=0;
            printf("5秒重发TIME\r\n");
            GSM_HTTP_INIT();
            gsm_post_get_time(&UsartType3);
        }
    }
    else
    {
        test2_cont=0;

    }
    if(Bin_System_task.open_push_flag==2)
    {
        test3_cont++;
        if(test3_cont>5000)
        {
            test3_cont=0;
            GSM_HTTP_INIT();
										test3_recont++;
					 if(test1_recont>5)
					 {
				    	test1_recont=0;
               Bin_System_task.get_card_qr_flag=0;
					    Bin_System.weight_end=0;
							Bin_System.weight_last=0;
						 Bin_System_task.get_info_flag=0;
						  	 red_led_open();
						     yellow_led_open();
						     green_led_close();
						   	 
						 
					 }
            gsm_post_openpsuh(&UsartType3,(char *)Bin_System.appid,
                              (char *)Bin_System.appkey,(char *)Bin_System.account,(char *)gsm_param.timestamp,MD5_data,
                              1,Bin_System.height,(char*)Bin_System.handle_time,(char*)Bin_System.device_id,(char*)Bin_System.door_no);
        }
    }
    else
    {	
			 test3_recont=0;
        test3_cont=0;
    }

}
uint8_t  rec_data_t[10]={0}; 
void poll_GSM_rec_data(USART_RECEIVETYPE *usart)
{
	  
   
    if(usart->RX_flag)
    {
        //收到数据
			  
			    for(uint16_t i=0;i<usart->RX_Size;i++)
			{
         printf("%c",usart->RX_pData[i]);
			}
//			 printf("收到数据=%d\r\n",usart->RX_Size);
        if(strstr((const char*)usart->RX_pData,(const char*)"+HTTPACTION"))
        {
            //读数据
					   memset(rec_data_t, 0,sizeof(rec_data_t));
					 printf("usart->RX_Size=%d",usart->RX_Size);
					  for(uint16_t i=0;i<(usart->RX_Size-23);i++)
					{
						 rec_data_t[i]=usart->RX_pData[i+21];  
					}	
					 
					printf(" rec_data_len=%s\r\n",rec_data_t);
            printf("发送读数据\r\n");
             read_gsm_data(rec_data_t);		
        }
//        //		printf("rec_buf=%s",rec_buf);
////        if(strstr((const char*)usart->RX_pData,(const char*)"timestamp"))
////        {
////            char *temp1;
////            temp1=strstr((const char*)usart->RX_pData,(const char*)"timestamp");
////            Bin_System_task.get_time_flag=3;
////            printf("temp=当前的时间:%s\r\n",temp1);
////            for(uint8_t i=0; i<10; i++)
////            {
////                Bin_System.timestamp[i]=*(temp1+12+i);
////            }
////            Bin_System.api_get_tim_flag=1;
////            printf("gsm_param->timestamp=当前的时间:");
////            for(uint8_t i=0; i<10; i++)
////            {
////                printf("%c", Bin_System.timestamp[i]);

////            }
////            printf("\r\n");
////            usart->RX_flag=0;
////            memset(usart->RX_pData, 0, usart->RX_Size);
////        }
//        //接收数据成功
//        //判断哪个数据
      if(Bin_System_task.get_info_flag==2)
             {

                if(strstr((const char*)usart->RX_pData,(const char*)"Success"))
                {

											 printf("收到数据2=%s\r\n",usart->RX_pData);
                        Bin_System_task.get_info_flag=3;
                        //  Bin_System.api_get_info_flag=1;
                        printf("USER is valid\r\n");
                        memset(usart->RX_pData, 0, usart->RX_Size);
                        usart->RX_flag=0;
								
                }
								 else if(strstr((const char*)usart->RX_pData,(const char*)"Undefined"))
                  
                {
                    Bin_System_task.get_info_flag=4;
                    printf("USER is no valid\r\n");
                    usart->RX_flag=0;
                    memset(usart->RX_pData, 0, usart->RX_Size);
									close_http(usart);
                }
              
             }
////        //OPEN PUSH
        if( Bin_System_task.open_push_flag==2)
        {
            if(strstr((const char*)usart->RX_pData,(const char*)"+HTTPREAD"))
            {
                if(strstr((const char*)usart->RX_pData,(const char*)"Success"))
                {
                    Bin_System_task.open_push_flag=3;
                     		close_http(usart);
                    printf("PUSH is valid\r\n");
                    memset(usart->RX_pData, 0,  usart->RX_Size);
                    usart->RX_flag=0;
                }
                usart->RX_flag=0;
            }
        }
       memset(usart->RX_pData, 0, sizeof(usart->RX_pData));
               usart->RX_flag=0;
				usart->RX_Size=0;

    }

  

}



void usart6_weight_rec_data(USART_RECEIVETYPE *usartxx,uint32_t *state,uint32_t *ad_data)
{
	     uint16_t check_1,check_2;
	  
	   if(usartxx->RX_flag)
		 {
			   check_1=usartxx->RX_pData[1]+usartxx->RX_pData[2]+usartxx->RX_pData[3]+
			           usartxx->RX_pData[4]+usartxx->RX_pData[5]+usartxx->RX_pData[6];
			   check_2=usartxx->RX_pData[7]*256+usartxx->RX_pData[8];
		
			    if((usartxx->RX_pData[0]==0xAA)&&(usartxx->RX_pData[9]==0xff))
		    { 
					 if(usartxx->RX_pData[1]==0xA3)
					 {
			         *state = (usartxx->RX_pData[4]*65536+ usartxx->RX_pData[5]*256+ usartxx->RX_pData[6])*10;//g
						  printf("state=%d",*state);
		       }
      else if(usartxx->RX_pData[1]==0xA1)
          {
               if(check_1==check_2)
							 {
								 *ad_data=usartxx->RX_pData[4]*65536+usartxx->RX_pData[6]*256+usartxx->RX_pData[6];
								 
							
							 }
					}	
				}	
      memset(usartxx->RX_pData, 0, usartxx->RX_Size);				
			          usartxx->RX_flag=0;
				 
			}
}

void all_usart_rec_data()
{
      poll_GSM_rec_data(&UsartType3);
	      if(Bin_System_task.get_last_height_flag==1)
				{
        usart6_weight_rec_data(&UsartType6,&Bin_System.weight_last,&Bin_System.weight_ad_last);
				}
				   if(Bin_System_task.start_time_flag==1)
				{
        usart6_weight_rec_data(&UsartType6,&Bin_System.weight_end,&Bin_System.weight_ad_last);
				}
				  
//				 if(Bin_System_task.overweight_flag==1)
//				 {
//					   usart6_weight_rec_data(&UsartType6,&Bin_System.weight_over,&Bin_System.weight_ad_last);
//					     
//					    printf("Bin_System.weight_over=%d\r\n",Bin_System.weight_over);
//				 }

      
				
}
void GSM_HTTP_INIT()
{
    gsm_register(&UsartType3);
}




/*****************
     test
****************/
void inity()
{
    Qr_code_state=Qr_code_idle;
}




