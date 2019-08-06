#ifndef __APP_H_
#define __APP_H_
#include "stm32f4xx_hal.h"
typedef enum
{
   Qr_code_idle = 0,
  Qr_code_busy
} Qr_code_state_type;

typedef enum
{
  system_idle = 0,   //¿ªÆôFRIDºÍÉ¨Âë
	system_busy = 1,
	system_result = 2
//  Qr_code_busy
}Smart_bin_system_state_type;

void GSM_HTTP_INIT();
 












#endif
