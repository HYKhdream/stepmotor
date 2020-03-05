/**
  *****************************************************************************
  * @file    hal_can.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-18
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

#ifndef __HAL_CAN_H__
#define __HAL_CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

/* Public typedef ------------------------------------------------------------*/

typedef struct {
	uint8_t l;
	uint8_t h;
}CMD_BYTE_t;

union UWORD {
	uint16_t word;
	CMD_BYTE_t byte;
};

typedef struct {
	uint8_t  type;
	uint8_t  data;
	union UWORD arg1;
	union UWORD arg2;
	union UWORD arg3;
}EXX_CMD_t;

/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

extern EXX_CMD_t ebrd_cmd;

/* Public function prototypes ------------------------------------------------*/

extern int hal_can_send(uint32_t msgid, uint8_t *dat, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_CAN_H__ */

