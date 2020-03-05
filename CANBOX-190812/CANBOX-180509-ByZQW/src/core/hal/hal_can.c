/**
  *****************************************************************************
  * @file    hal_can.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-18
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "hal_can.h"

/* Private typedef -----------------------------------------------------------*/

typedef struct {
	uint16_t msgid;
	uint16_t dlc;
	uint8_t *pdata;
} CAN_PKG_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

EXX_CMD_t ebrd_cmd;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

int hal_can_send(uint32_t msgid, uint8_t *dat, uint8_t len)
{
	//static long ntime = 0;
	long ctime;
	
	while (1)
	{
	#if 0
		ctime = arch_get_systicks();
		if (ntime < ctime)
			break;
		#endif
		if(arch_can_delay_nx100us(5))
		{
			break;
		}
	}
	//ntime = ctime + 5;
	
	arch_can_send(msgid, len, dat);
	
	return 0;
}

