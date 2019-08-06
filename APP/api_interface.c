

/**
 * @file   api_interface.c
 * @brief  �͵ײ�Խ�
 * @details ϸ��
 * @mainpage ���̸���
 * @author Fubiao.Guo
 * @email  2545188523@qq.com
 * @version V1.0
 * @date 2019/7/19
 * @license ���������ҿƼ����޹�˾
 */

#include "api_interface.h"
#include "stm32f4xx_hal.h"

/*************************************
          ״̬ƥ��
**************************************/
void state_state()
{
	
//Bin_System.Qr_code_data_flag=SCANER_RX_STA;
	
}

/***********************************
   ��ӡ��API
************************************/

/**
 * @brief print_qr  ��ά���ӡ����
 * @param pbuf ��ӡ������
 * @return void
 * @retval void
 */
void print_qr(CallBackFun pcallback,char *pbuf)
{
	       pcallback(pbuf);
} 

/***********************************
    ɨ���ά��
***********************************/

/**
 * @brief scan_qr  ��ȡɨ���ά��
 * @param pbuf     ��ά������
 * @return void
 * @retval void
 */
void scan_qr(CallBackFun pcallback,char *pbuf)
{
	        pcallback(pbuf);
}
/***********************************
     ���ӳ���
***********************************/

/**
 * @brief get_height_data  ��ȡ����
 * @param pbuf    ����
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
        �����
***********************************/

/**
 * @brief open_door  ����
 * @param  void
 * @return void
 * @retval void
 */
//void open_door(CallBackFun pcallback)
//{
//	       pcallback(0);
//}
/***********************************
          webͨ��
***********************************/

/**
 * @brief get_web_data  ��ȡweb����
 * @param pbuf    ����web����
 * @return void
 * @retval void
 */
void get_web_data(CallBackFun pcallback,char *p)
{
	       pcallback(p);
}

/**
 * @brief get_web_data  ����web����
* @param pbuf    ����web����
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
	 ����
	*******/

}