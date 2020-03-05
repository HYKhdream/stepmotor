/*
 * arch_can.c	(by xhl 2014/10/31)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f37x.h"

CanRxMsg RxMessage;
volatile int can_receive_count;

void CAN_Config(int GET_ID)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* CAN GPIOs configuration **************************************************/

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_9);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_9); 

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* NVIC configuration *******************************************************/
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

#if 0
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

	/* CAN configuration ********************************************************/  
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

	/* CAN Baudrate = 1MBps (CAN clocked at 36 MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1, &CAN_InitStructure);

	/* CAN filter init "FIFO0" */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = GET_ID<<5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

#if 0
	/* CAN filter init "FIFO1" */
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x702<<5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
#endif

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
      can_receive_count=0;
	/* Enable FIFO 1 message pending Interrupt */
	//CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);

}

#define ACTION_ALARM	0
#define ACTION_DATA	1
#define ACTION_LOG	2


extern int Get_board_id();

void arch_SendMassage(unsigned int action, unsigned int *pMsg, unsigned int len)
{
	CanTxMsg TxMessage;
	unsigned short *pData;
	unsigned int idx;
	unsigned int canresendcount;

	if(len == 0) 
		return;

	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = len * 2L;
	TxMessage.IDE = CAN_ID_STD;
	if(Is_Yarn_Board())
	{
		switch(action) {
		case ACTION_ALARM:
			TxMessage.StdId = 0x00000770+(3*Get_board_id()); // CAN ID   
			break;
		case ACTION_DATA:
			TxMessage.StdId = 0x00000772+(3*Get_board_id()); // CAN ID
			break;
		}

	}
	else
	{
		if(Step_is_CX_5N_board())
			{
		switch(action) {
		case ACTION_ALARM:			
		case ACTION_DATA:
			TxMessage.StdId = Get_board_id()? (0x00000632+(3*Get_board_id())):0x00000732; // CAN ID
			break;
		case ACTION_LOG:
			TxMessage.StdId = 0x00000600; 	
			break;
		}
			}
		else
			{
		switch(action) {
		case ACTION_ALARM:
			TxMessage.StdId = Get_board_id() ? (0x00000630+(3*Get_board_id())):0x00000730 ; // CAN ID
			break;
		case ACTION_DATA:
			TxMessage.StdId = Get_board_id()? (0x00000632+(3*Get_board_id())):0x00000732; // CAN ID
			break;
		case ACTION_LOG:
			TxMessage.StdId = 0x00000600; 	
			break;
		}
			}
	}
	
	pData = (unsigned short *)&TxMessage.Data;
	do {
		*pData++ = *pMsg++;
	}while(--len);

	canresendcount =0 ;

again:
	if((idx = CAN_Transmit(CAN1, &TxMessage)) == CAN_NO_MB) {

		canresendcount++;
		//arch_delay_us(100);

		if (canresendcount>1000)
		{
			//can_send_error =1;
			return;	
		}
		else
		goto again;
	}

	//
	//while(CAN_TransmitStatus(idx) != CANTXOK);
}

#if 0
int Massage_Push(unsigned int cmd, unsigned int arg1, unsigned int arg2);
#else
//int Massage_Push(unsigned int cmd, unsigned int arg1, unsigned int arg2, unsigned int arg3,unsigned int arg4);
int Massage_Push(unsigned short cmd, 	unsigned short arg1, unsigned short arg2, unsigned short arg3, unsigned short arg4);
#endif
void hook_CAN_isr(void)
{
	//CanRxMsg RxMessage;
	unsigned short *pData = (unsigned short *)&RxMessage.Data;
//	int Massage_Push(unsigned short cmd, unsigned short arg1, unsigned short arg2);
	int i;

	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
	//if(RxMessage.StdId == 0x0320){
		can_receive_count ++;

		//if(RxMessage.DLC > 1) {
		//myprintf("CAN isr DLC %d, 0x%x\r\n", RxMessage.DLC, RxMessage.Data[0]);
		//}
		for(i = RxMessage.DLC; i < 8; i ++) RxMessage.Data[i] = 0x0;		

#if 0
		Massage_Push(RxMessage.Data[0], RxMessage.Data[1], pData[1]);
#else
		Massage_Push(RxMessage.Data[0], RxMessage.Data[1], pData[1], pData[2],pData[3]);
#endif
	//}
}



