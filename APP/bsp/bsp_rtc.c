#include "bsp_rtc.h"
#include "string.h"
#include "rtc.h"
#include "stdio.h"
extern RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef control_time;
RTC_DateTypeDef control_date;
const uint8_t Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint16_t monDays[12] = {0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
uint32_t time2Stamp(RTC_DateTypeDef date, RTC_TimeTypeDef time)    //北京时间转时间戳
{
	uint32_t result;
	uint16_t Year=date.Year+2000;
	result = (Year - 1970) * 365 * 24 * 3600 + (monDays[date.Month-1] + date.Date - 1) * 24 * 3600 + (time.Hours-8) * 3600 + time.Minutes * 60 + time.Seconds;
//	printf("[%u]",result);
	result += (date.Month>2 && (Year % 4 == 0) && (Year % 100 != 0 || Year % 400 == 0))*24*3600;	//闰月
//	printf("[%u]",result);
	Year -= 1970;
	result += (Year/4 - Year/100 + Year/400)*24 * 3600;		//闰年
	return result;
}

  char str_time[40];
 /**
  * @brief  Get the current time 
  * @param  void
  * @param  void
  * @return void
  * @retval void
  */
uint32_t get_timer()
{
	   memset(str_time, 0, sizeof(str_time)); 
	                
	if(HAL_RTC_GetDate(&hrtc,&control_date, RTC_FORMAT_BIN)==HAL_OK)
	{
//		 printf("/***************************************************/\r\n");
	// printf("%d年%d月%d日\r\n",control_date.Year,control_date.Month,control_date.Date);
//		   control_date.Year  = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
//       control_date.Month= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
//       control_date.Date  = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
  //北京时间转时间戳
//            char string[2];
//		    sprintf(string,"%02d", control_date.Year);
//		       strcpy (str_time,"20");
//		       strcat (str_time,string);
//	         strcat (str_time,"/");
//		    sprintf(string, "%02d", control_date.Month);
//		       strcat (str_time,string);
//	         strcat (str_time,"/");
//		    sprintf(string, "%02d", control_date.Date);
//			     strcat (str_time,string);
//	         strcat (str_time," ");

//   printf("string=%s\n",str_time);
 
//		  
//    strcpy (str,"these");
//    strcat (str,"strings ");
//    strcat (str,"are ");
//    strcat (str,"concatenated.");
//		printf("%s",str);
	}
 if(HAL_RTC_GetTime(&hrtc, &control_time, RTC_FORMAT_BIN)==HAL_OK)
 {
	 
	 
 // printf("%d时%d分%d秒\r\n",control_time.Hours,control_time.Minutes,control_time.Seconds);
//	     char string1[2];
//		    sprintf(string1,"%02d", control_time.Hours);
//		       strcat (str_time,string1);
//	 strcat (str_time,":");
//		    sprintf(string1, "%02d", control_time.Minutes);
//		       strcat (str_time,string1);
//	 strcat (str_time,":");
//		    sprintf(string1, "%02d", control_time.Seconds);
//			      strcat (str_time,string1);
	 //加个空格,可以去掉最后一位，当占位
//	          strcat (str_time," ");
//	 strcat (str_time,"");

 }
	 return time2Stamp(control_date, control_time);
}
/**
 * @brief  set time
 * @param Year()
 * @param 形参 参数说明
 * @return void
 * @retval void
 */
void set_time_data(uint16_t Year,uint16_t Month,uint16_t Date,uint16_t Hours, uint16_t Minutes ,uint16_t Seconds)
{
	  control_date.Year=Year;
	  control_date.Month=Month;
	  control_date.Date=Date;
	  control_time.Hours=Hours;
	  control_time.Minutes=Minutes;
	  control_time.Seconds=Seconds;
	 HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2,  control_date.Year);
   HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, control_date.Month );
   HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, control_date.Date);
	 HAL_RTC_SetDate(&hrtc,&control_date,RTC_FORMAT_BIN);
	 HAL_RTC_SetTime(&hrtc,&control_time,RTC_FORMAT_BIN);
}


 


