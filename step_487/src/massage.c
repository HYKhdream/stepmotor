#include <stdio.h>
//#include "type.h"
#include "massage.h"
#include "alert.h"
//#include "arch.h"	
#include "command.h"	

#define MAX_MASSAGE	80
MASSAGE_TYPE MsgBuff[MAX_MASSAGE];
volatile unsigned int msg_rptr, msg_wptr;

void Massage_Init()
{
	msg_rptr = msg_wptr = 0;
/*	
	memset(MsgBuff, 0, MAX_MASSAGE * sizeof(MASSAGE_TYPE));
*/
}

int Massage_Count()
{
	int count = msg_wptr;
	count += MAX_MASSAGE;
	count -= msg_rptr;
	if(count >= MAX_MASSAGE) {
		count -= MAX_MASSAGE;
	}

	return count;
}

#if 1//def PLATFORM_TMS2812
int Massage_Push(unsigned short cmd, 
	unsigned short arg1, unsigned short arg2, unsigned short arg3,unsigned short arg4)
{
	if(Massage_Count() >= MAX_MASSAGE - 1) {
		alert_push(CAN_BUF_ERR, 0);
		return -1;
	}

	MsgBuff[msg_wptr].cmd = cmd;
	MsgBuff[msg_wptr].arg1 = arg1;
	MsgBuff[msg_wptr].arg2 = arg2;
	MsgBuff[msg_wptr].arg3 = arg3;
	MsgBuff[msg_wptr].arg4 = arg4;
	

	msg_wptr ++;
	if(msg_wptr >= MAX_MASSAGE) {
		msg_wptr -= MAX_MASSAGE;
	}
	
	return 0;
}
#else
int Massage_Push(unsigned short cmd, unsigned short arg1, unsigned short arg2)
{
	if(Massage_Count() >= MAX_MASSAGE - 1) {
		alert_push(CAN_BUF_ERR, 0);
		return -1;
	}

	MsgBuff[msg_wptr].cmd = cmd;
	MsgBuff[msg_wptr].arg1 = arg1;
	MsgBuff[msg_wptr].arg2 = arg2;

	msg_wptr ++;
	if(msg_wptr >= MAX_MASSAGE) {
		msg_wptr -= MAX_MASSAGE;
	}
	
	return 0;
}
#endif
MASSAGE_TYPE *Massage_Pop()
{
	if(Massage_Count() > 0) {
		int rptr = msg_rptr;
		msg_rptr ++;
		if(msg_rptr >= MAX_MASSAGE) {
			msg_rptr -= MAX_MASSAGE;
		}
		return &MsgBuff[rptr];
	}
	return NULL;
}

#define ACTION_ALARM	0
#define ACTION_DATA	1
#define ACTION_LOG     2


void arch_SendMassage(unsigned int action, unsigned int *pMsg, unsigned int len);

void Massage_Send(unsigned int Msg)
{
	arch_SendMassage(ACTION_DATA, &Msg, 1);
}

void Massage_Send_4Word(unsigned short Msg1,unsigned short Msg2,unsigned short Msg3,unsigned short Msg4)
{
	unsigned int buff[4];

	buff[0] = Msg1;
	buff[1] = Msg2;
	buff[2] = Msg3;
	buff[3] = Msg4;
	
	arch_SendMassage(ACTION_DATA, buff, 4);
}


void Massage_Send_4Word_log(unsigned short Msg1,unsigned short Msg2,unsigned short Msg3,unsigned short Msg4)
{
	unsigned int buff[4];

	buff[0] = Msg1;
	buff[1] = Msg2;
	buff[2] = Msg3;
	buff[3] = Msg4;
	
	arch_SendMassage(ACTION_LOG, buff, 4);
}


void Massage_Send_Alert(unsigned int Msg, unsigned int arg1)
{
	unsigned int buff[2];
	if(Step_is_CX_5N_board())
		{
	buff[0] = 0x00F0;
	buff[1] = Msg;
	buff[2] = arg1;
	arch_SendMassage(ACTION_ALARM, buff, 3);
		}
	else
		{
	buff[0] = Msg;
	buff[1] = arg1;
	arch_SendMassage(ACTION_ALARM, buff, 2);
		}
	
}

#ifdef LOGOUT_ENABLE

extern volatile unsigned short logout_needle_pos;

void Massage_Send_Log_Sti(unsigned int cmd,
	unsigned int curPos, unsigned int dstPos)
{	
	unsigned int buff[4];	
	buff[0] = cmd;	
	buff[1] = logout_needle_pos;	
	buff[2] = curPos;	
	buff[3] = dstPos;		
	arch_SendMassage(ACTION_LOG, buff, 4);
}

void Massage_Send_Log_Dct(unsigned int cmd,
	unsigned int group, unsigned int index, unsigned int status)
{	
	unsigned int buff[4];	

	buff[0] = cmd;	
	buff[1] = logout_needle_pos;	
	if(cmd == LOG_OUT_YARN)	{		
		buff[2] = index;	
	}	
	else {		
		buff[2] = (group << 8) | index;		
	}		
	buff[3] = status;
	arch_SendMassage(ACTION_LOG, buff, 4);
}
#endif

