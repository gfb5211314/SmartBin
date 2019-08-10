#ifndef   __BSP_WEIGHT_H__
#define   __BSP_WEIGHT_H__

#include "stm32f4xx_hal.h"


void get_ad_weight_command(void);

void  calibration_weight_zero_command(void);

/**
 * @brief calibration weight sensor
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 * @retval 返回值说明
 */
void  get_weight_command(void);

void get_ad_weight_command(void);




void USART6_IRQHandler(void);

void DMA_USART6_IDLE_INIT(void);

uint8_t  u6_unpack(void);  //不能定义 uint8_t state  ，否则传不了参数

/***get  g**************/
void  get_weight_value(uint32_t * state);

#endif