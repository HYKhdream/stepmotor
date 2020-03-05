/**
  *****************************************************************************
  * @file    CanFifo.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-17
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

#ifndef __CANFIFO_H__
#define __CANFIFO_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Public typedef ------------------------------------------------------------*/

typedef struct
{
	unsigned long msg_id;
	unsigned char msg_dat[8];
	unsigned char msg_len;
	unsigned short msg_time;
} CAN_MSG_DEF;



/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/

int CanRecv_Clear();
int CanRecv_TopIdx();
int CanRecv_Count();
unsigned long CanRecv_Total();
int CanRecv_FramePrint_loc(int loc, char *buf);
void arch_can_send(unsigned long MsgID, int len, unsigned char *data);
void ShowFifo_push(unsigned long MsgID, int len, unsigned char *data);
int can_bps_get(void);
void do_filter(void);
void can_buf_init();
void can_fifo_para_load();
extern int CanRecv_pick(int loc, CAN_MSG_DEF **Msg);
int can_fifo_push(unsigned long ulMsgID, int MsgLen, unsigned char *Msg);
int CanRecv_status();
int CanRecv_Data(unsigned long *tMsgID,unsigned char *tMsg);
void can_data_read(unsigned long ulMsgID, int MsgLen, unsigned char *Msg);
#ifdef __cplusplus
}
#endif

#endif /* __CANFIFO_H__ */

