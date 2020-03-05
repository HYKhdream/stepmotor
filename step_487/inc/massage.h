
#ifndef __MASSAGE_H__
#define __MASSAGE_H__

typedef struct {
	unsigned short cmd;
	unsigned short arg1;
	unsigned short arg2;
#if 1//def PLATFORM_TMS2812
	unsigned short arg3;
	unsigned short arg4;
#endif
}MASSAGE_TYPE;

void Massage_Init(void);
int Massage_Count(void);
#if 1//def PLATFORM_TMS2812
int Massage_Push(unsigned short cmd, 
	unsigned short arg1, unsigned short arg2, unsigned short arg3, unsigned short arg4);
#else
int Massage_Push(unsigned short cmd, unsigned short arg1, unsigned short arg2);
#endif
MASSAGE_TYPE *Massage_Pop(void);
void Massage_Send(unsigned int Msg);
void Massage_Send_Alert(unsigned int Msg, unsigned int arg1);
void Massage_Send_4Word(unsigned short Msg1,unsigned short Msg2,unsigned short Msg3,unsigned short Msg4);

#endif

