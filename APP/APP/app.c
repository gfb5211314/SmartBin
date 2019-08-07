
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

//#define   open_rtc   0
typedef struct{
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
	uint16_t weight_last;
	uint16_t weight_end;
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
	/*************函数*********************/

}Bin_System_Type;

typedef struct{
 uint8_t	 get_card_qr_flag;
 uint8_t	 get_time_flag;
 uint8_t   get_info_flag;
 uint8_t   get_last_height_flag;
 uint8_t   open_door_flag;
 uint8_t   start_time_flag;	
 uint8_t   open_push_flag;
}Bin_System_task_Type;
Bin_System_task_Type  Bin_System_task;
extern USART_RECEIVETYPE  UsartType3;
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
	
    get_ntp_time(&UsartType3.RX_flag,UsartType3.RX_pData);
	sprintf ((char*)Bin_System.handle_time,"20%d-%02d-%02d %02d:%02d:%02d",gsm_param.ALT_year,gsm_param.ALT_month,
	gsm_param.ALT_day,gsm_param.ALT_hour,gsm_param.ALT_minute,gsm_param.ALT_second);
      sprintf(Bin_System.timestamp,"%d",get_timer());
}
void System_ilde_task()
{    
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
				} 
else
{
	get_timer();
  memset(Bin_System.account, 0, sizeof(Bin_System.account));

}	
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
		if((Bin_System_task.get_info_flag==1)&&(Bin_System_task.get_time_flag==3))
		{
			   #ifdef  open_rtc
			 if(gsm_post_getinfo(&UsartType3.RX_flag,UsartType3.RX_pData,(char *)Bin_System.appid,(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data))
			 {
				       //等待数据回应
				   printf("send user jiaonian \r\n");
				   Bin_System_task.get_info_flag=2;
			 }
				#else
					 if(gsm_post_getinfo(&UsartType3.RX_flag,UsartType3.RX_pData,(char *)Bin_System.appid,(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data))
			 {
				       //等待数据回应
				   printf("send user jiaonian \r\n");
				   Bin_System_task.get_info_flag=2;
			 }
			 #endif 
		}

		//获取称重任务完成
   if(Bin_System_task.get_last_height_flag==1)
	 {

		     if(u6_unpack())
				 {
					   printf("获得重量 \r\n");
					  Bin_System_task.get_last_height_flag=2;
           get_weight_value(&Bin_System.weight_last);
           
				 
				 } 
				 else
				 {
//					    printf("get weight \r\n");
					 
					 get_weight_command();	 
					   osDelay(50);
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
		if(Bin_System_task.get_info_flag==4)
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
		if(Bin_System_task.open_door_flag==1)
		{
			printf("door is open");
		  Bin_System_task.open_door_flag=0;
		   	  open_door();
			Bin_System_task.start_time_flag=1;
			
		}
		//开启30秒检测重量
		if(Bin_System_task.start_time_flag==1)
		{
			   printf("30秒进入\r\n");
			  if(u6_unpack())
				 {
					 printf("获得zhonglshuju");
           get_weight_value(&Bin_System.weight_end);
					  printf("Bin_System.weight_end=%d",Bin_System.weight_end);
					   printf("Bin_System.weight_last=%d",Bin_System.weight_last);
  
          if((Bin_System.weight_end-Bin_System.weight_last)>30)
					{		
 	Bin_System.height=Bin_System.weight_end-Bin_System.weight_last;						
						  printf("已经丢垃圾\r\n");
				       Bin_System_task.start_time_flag=0;
						   Bin_System_task.open_push_flag=1;
				  } 
				 }
				 else
				 {
					 get_weight_command();	 
					  osDelay(50);
					    
				 }
				 			
		}
		if(Bin_System_task.open_push_flag==1)
		{
			printf("推送垃圾信息\r\n");
			    #ifdef  open_rtc 
			   
			           
			    #else
			         timer_update();    
			    #endif
				 if(gsm_post_openpsuh(&UsartType3.RX_flag,UsartType3.RX_pData,(char *)Bin_System.appid,(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data,
					 1,Bin_System.height,Bin_System.handle_time,Bin_System.device_id,Bin_System.door_no))
			   {
				       //等待数据回应
				   Bin_System_task.open_push_flag=2;
			   }
			
		}
		//流程完成
	 if(Bin_System_task.open_push_flag==3)
	 {
		 printf("liuchengwancheng\r\n");
		 Bin_System_task.open_push_flag=0;
		 Bin_System_task.get_card_qr_flag=0;
	 }

//	//获得时间,发送请求用户信息
//  if(Bin_System.api_get_tim_flag==1)
//	{

//			gsm_post_getinfo(&UsartType3.RX_flag,UsartType3.RX_pData,(char *)Bin_System.appid,(char *)Bin_System.appkey,(char *)Bin_System.Card_data,(char *)gsm_param.timestamp,MD5_data); 

//	}
//	//用户比对成功
//  if(Bin_System.api_get_info_flag==1)	
//	{
//      open_door();
//		Bin_System.door_flag=1;
//	}
//	if(	Bin_System.door_flag==1)
//	{
//		    Bin_System.door_open_tim_flag=1;
//	}
//	if(Bin_System.door_open_tim_flag==1)
//	{
//		    Bin_System.door_open_tim_count++;
//		       if(u6_unpack())
//				 {
//               get_weight_value(Bin_System.weight_end);
//           	 if((Bin_System.weight_end-Bin_System.weight_last)>30)
//						 {
//						if(gsm_post_openpsuh(&UsartType3.RX_flag,UsartType3.RX_pData,
//							 (char *)Bin_System.appid,(char *)Bin_System.appid,appkey,
//								 Bin_System.account,Bin_System.timestamp,MD5_data,
//	           	Bin_System.type,Bin_System.height,Bin_System.handle_time,
//							 Bin_System.device_id,Bin_System.door_no))
//							 {
//								 //标志发送成功 ,wait ack
//								    Bin_System.open_push_sucess=1;
//								 
//							 }
//						
//						}
//				 }						 
//		            //超时30秒
//		  if(Bin_System.door_open_tim_count>30000)
//			{
//				  close_door();
//				 	Bin_System.door_timout_flag=1;
//			}
//		
//	}
//  if(	Bin_System.door_timout_flag==1)
//	{
//		
//		
//		
//		
//	}
}
uint16_t test1_cont=0;
uint16_t test2_cont=0;
uint16_t test3_cont=0;
//任务重发 5秒重发
void resend_task()
{

	
	if(Bin_System_task.get_info_flag==2)
	{
		test1_cont++;
		 if(test1_cont>5000)
		 {
		test1_cont=0;
		printf("5miao send user jiaonian \r\n");
			  	 GSM_HTTP_INIT();
		gsm_post_getinfo(&UsartType3.RX_flag,UsartType3.RX_pData,(char *)Bin_System.appid,
			(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)Bin_System.timestamp,MD5_data);
		}
	}
	else
	{
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
		gsm_post_get_time(&UsartType3.RX_flag,UsartType3.RX_pData);
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
	   	gsm_post_openpsuh(&UsartType3.RX_flag,UsartType3.RX_pData,(char *)Bin_System.appid,
			(char *)Bin_System.appkey,(char *)Bin_System.account,(char *)gsm_param.timestamp,MD5_data,
					 1,Bin_System.height,Bin_System.handle_time,Bin_System.device_id,Bin_System.door_no);
					 }
	}
	else
	{
		test3_cont=0;
	}
	
}

void poll_GSM_rec_data(uint8_t *Receive_data_flag,uint8_t *rec_buf)
{
	 
	if(Receive_data_flag)
	{

       //收到数据
     if(strstr((const char*)rec_buf,(const char*)"+HTTPACTION: 1,"))
        {
					//读数据

            send_command((char *)"AT+HTTPREAD\r\n","+HTTPREAD:",20,Receive_data_flag,rec_buf);
				printf("rec_buf=%s",rec_buf);
					   if(strstr((const char*)rec_buf,(const char*)"timestamp"))
						 {
							     char *temp1;
							   temp1=strstr((const char*)rec_buf,(const char*)"timestamp");
							       Bin_System_task.get_time_flag=3;
							   printf("temp=当前的时间:%s\r\n",temp1);
							  for(uint8_t i=0;i<10;i++)
							 {
							   Bin_System.timestamp[i]=*(temp1+12+i);
							 }
							 Bin_System.api_get_tim_flag=1;
							 printf("gsm_param->timestamp=当前的时间:");
							 for(uint8_t i=0;i<10;i++)
							 {
							  	 printf("%c", Bin_System.timestamp[i]);
							
							 }
							    printf("\r\n");
							 	*Receive_data_flag=0;
							  memset(rec_buf, 0, sizeof(*rec_buf));
						 }
						 //接收数据成功
				    //判断哪个数据
							 if( Bin_System_task.get_info_flag==2)
							 {
								 		 if(strstr((const char*)rec_buf,(const char*)"Success"))
						    {   
							  
						     if(strstr((const char*)rec_buf,(const char*)"district"))
								 {     
									    Bin_System_task.get_info_flag=3;
								    //  Bin_System.api_get_info_flag=1;
							        printf("USER is valid\r\n");
									       memset(rec_buf, 0, sizeof(*rec_buf));
									 	*Receive_data_flag=0;
								 }
							 }
							  else if(strstr((const char*)rec_buf,(const char*)"Undefined"))
						 
						
									{
										  Bin_System_task.get_info_flag=4;
										 printf("USER is no valid\r\n");
										*Receive_data_flag=0;
										 memset(rec_buf, 0, sizeof(*rec_buf));  
									}
									*Receive_data_flag=0;
							 }
								 //OPEN PUSH
								 if( Bin_System_task.open_push_flag==2)
								 {
									 
								  if(strstr((const char*)rec_buf,(const char*)"Success"))
								 {     
									   Bin_System_task.open_push_flag=3;
							
							        printf("PUSH is valid\r\n");
									    memset(rec_buf, 0, sizeof(*rec_buf));  
		            	*Receive_data_flag=0;									 
								 }
								 *Receive_data_flag=0;
							 }
							 *Receive_data_flag=0;
								   UsartType3.RX_flag=0;
							  memset(UsartType3.RX_pData, 0, sizeof(UsartType3.RX_pData));  	
			 }
					
					
						 
		}
			
	
	} 
	

void all_usart_rec_data()
{
	poll_GSM_rec_data(&UsartType3.RX_flag,UsartType3.RX_pData);
}
void GSM_HTTP_INIT()
{
	gsm_register(&UsartType3.RX_flag,UsartType3.RX_pData);
}
/**
 * @brief System_task  主任务流程
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */
void System_task(Smart_bin_system_state_type  Smart_bin_system_state)
{
		switch(Smart_bin_system_state)
		{
			//任务空闲状态   -循环扫描 FRID 和扫码机
			case system_idle :
				           //刷卡或者扫描二维码
				        if((Bin_System.Card_Check==1)||(Bin_System.Qr_code_check==1))
								{
									     
									   Smart_bin_system_state =system_busy;
									
								}
								
  				break;
			//系统进入处理状态				
	    case system_busy :
															 //刷卡或者扫描二维码
								if((Bin_System.Card_Check==1)||(Bin_System.Qr_code_check==1))
							{
															
																 Smart_bin_system_state =system_result;
															
									}
										
				break;
				 //系统结果状态				
	    case system_result :
															 //刷卡或者扫描二维码
								if((Bin_System.Card_Check==1)||(Bin_System.Qr_code_check==1))
							{
															
																 Smart_bin_system_state =system_idle;
															
								}
										
				break;
			
			
		}
}

/**
 * @brief door_task  主任务流程
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */
void door()
{
}
/**
 * @brief System_task_demo  主任务流程
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */



void System_task_demo(Bin_System_Type Bin_System_temp)
{
	
	

	
}

/*****************
     test
****************/
void inity()
{
	Qr_code_state=Qr_code_idle;
}

void system_demo()
{
	    	

	  
		switch (Qr_code_state)
    {
			 

      	case Qr_code_idle :  
//获取称重状态
					if(Bin_System.height_flag)
					{
              Bin_System.height_flag=0;   
              get_weight_value(&Bin_System.weight_last);
						printf("未丢垃圾的重量为:%d克\r\n",Bin_System.weight_last);
             
					}		
         else
				  {
            //检测数据是否有数据
						  get_weight_command();
            u6_unpack(&Bin_System.height_flag);
//						printf("Bin_System.height_flag=%d",Bin_System.height_flag);

				  }						
					if(Bin_System.Qr_code_data_flag)
				{
					  printf("获取到用户二维码信息\r\n");
					    Bin_System.Qr_code_data_flag=0;
             u2_data_memy(Bin_System.Qr_code_data,&Bin_System.Qr_code_data_len);
//					  printf("Bin_System.Qr_code_data=%s",Bin_System.Qr_code_data);
             
					     //获取为丢垃圾的重量
					 
					  Bin_System.print_state_flag=0;
					    Qr_code_state=Qr_code_busy;
				}	
 				 else
				 {
	
					   u2_unpack(&Bin_System.Qr_code_data_flag);
//					  printf("Bin_System.Qr_code_data_flag=%d\r\n",Bin_System.Qr_code_data_flag);
				 }
				 				 

                    			
			                                               
    		                          break;
    	case Qr_code_busy :
//				      printf("jinru");
			                /*****start print***********/
			                if(Bin_System.print_state_flag==0)
											{
												 Bin_System.print_state_flag=1;
				             print_qr_data(Bin_System.Qr_code_data,Bin_System.Qr_code_data_len);
//												  osDelay(500);
												//设置定时器标志启动计算时间
												printf("开启30秒计数\r\n");
												     set_tim_flag(1);
												 Bin_System.tim_count_flag=1;
											}
											//进入检测垃圾状态
			                if(Bin_System.tim_count_flag==1)
											{
												
												
											   	 get_timout_flag(&Bin_System.door_timout_flag);
												   if(Bin_System.door_timout_flag==0)
													 {
												
														 //检测垃圾重量
                           	if(Bin_System.height_flag)
					                  {
																	  printf("jinlai");
                                 Bin_System.height_flag=0;   
                              get_weight_value(&Bin_System.weight_end);
													uint16_t	difference_data=Bin_System.weight_end-Bin_System.weight_last;
																printf("未丢垃圾的重量为:%d克\r\n",Bin_System.weight_last);
																printf("丢垃圾的重量为:%d克\r\n",Bin_System.weight_end);
															//大于30克
					                    if((difference_data>20)&&(Bin_System.weight_end>Bin_System.weight_last))
															{
																  //关闭定时器技术
                                  set_tim_flag(0);
																 Bin_System.tim_count_flag=0;
                               	printf("丢垃圾成功\r\n");
																 Qr_code_state=Qr_code_idle;
																 printf("业务完成\r\n");
				            	        }
                              else
															{
                                 printf("请丢垃圾\r\n");
															}																
														}															
													else
															{
																		 //检测数据是否有数据
																		 get_weight_command();
//																	u4_unpack(&Bin_System.height_flag);

															 }
														 }
													else{
																set_timout_flag(0);
														 set_tim_flag(0);
																	Bin_System.tim_count_flag=0;
																 Qr_code_state=Qr_code_idle;
																printf("用户没丢垃圾\r\n");
																printf("业务失败\r\n");
																}	
												

											   }												
				             
				
    	                  	     break;
    	   default:
    		 break;
    }
	
}



