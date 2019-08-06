

/**
 * @file   api_interface.c
 * @brief  和底层对接
 * @details 细节
 * @mainpage 工程概览
 * @author Fubiao.Guo
 * @email  2545188523@qq.com
 * @version V1.0
 * @date 2019/7/19
 * @license 深圳市琴弦科技有限公司
 */

#include "api_interface.h"
#include "stm32f4xx_hal.h"

/*************************************
          状态匹配
**************************************/
void state_state()
{
	
//Bin_System.Qr_code_data_flag=SCANER_RX_STA;
	
}

/***********************************
   打印机API
************************************/

/**
 * @brief print_qr  二维码打印函数
 * @param pbuf 打印的数据
 * @return void
 * @retval void
 */
void print_qr(CallBackFun pcallback,char *pbuf)
{
	       pcallback(pbuf);
} 

/***********************************
    扫描二维码
***********************************/

/**
 * @brief scan_qr  获取扫描二维码
 * @param pbuf     二维码数据
 * @return void
 * @retval void
 */
void scan_qr(CallBackFun pcallback,char *pbuf)
{
	        pcallback(pbuf);
}
/***********************************
     电子称重
***********************************/

/**
 * @brief get_height_data  获取数据
 * @param pbuf    重量
 * @return void
 * @retval void
 */
void get_height_data(CallBackFun pcallback,char *pbuf)
{
	       pcallback(pbuf);
}
void height_chang(CallBackFun pcallback,char *pbuf)
{
	       pcallback(pbuf);
}
/***********************************
        电磁门
***********************************/

/**
 * @brief open_door  打开门
 * @param  void
 * @return void
 * @retval void
 */
//void open_door(CallBackFun pcallback)
//{
//	       pcallback(0);
//}
/***********************************
          web通信
***********************************/

/**
 * @brief get_web_data  获取web数据
 * @param pbuf    接收web数据
 * @return void
 * @retval void
 */
void get_web_data(CallBackFun pcallback,char *p)
{
	       pcallback(p);
}

/**
 * @brief get_web_data  发送web数据
* @param pbuf    发送web数据
 * @return void
 * @retval void
 */
void send_web_data(CallBackFun pcallback,char *p)
{
	       pcallback(p);
}
void  message_jiaoyan(char *p)
{
	/*******
	 代码
	*******/

}