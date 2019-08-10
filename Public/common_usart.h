#ifndef __COMMON_USART_H__
#define __COMMON_USART_H__

#include "stm32f4xx_hal.h"

#define  RX_LEN  2048
typedef struct
{
    uint8_t  RX_flag;        //IDLE receive flag
    uint16_t RX_Size;          //receive length
	  uint16_t tem_RX_Size;          //receive length
    uint8_t  RX_pData[RX_LEN];
	  uint8_t  tem_RX_pData[RX_LEN];
    uint8_t  TX_pData[100];
} USART_RECEIVETYPE;







#endif
