/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "guilib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/sysctl.h"
#include "inc/timer.h"
#include "inc/can.h"

#include "canfifo.h"

void arch_hook_100us(void);
void can_send_cont(void);
void arch_hook_100us(void);

void Timer0A_Handler()
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	arch_hook_100us();
}

void Timer1B_Handler_delay(void)
{
	
	TimerIntClear(WTIMER1_BASE, TIMER_TIMB_TIMEOUT);
}
void Timer2B_Handler(void)
{
	TimerIntClear(WTIMER2_BASE, TIMER_TIMB_TIMEOUT);
}


void CAN_GetMessage(unsigned long ulBase, unsigned long ulObjID)
{
	tCANMsgObject CANMessage;
	unsigned char ucMsgData[8];

	CANMessage.pucMsgData = ucMsgData;

	CANMessageGet(ulBase, ulObjID, &CANMessage, 1);

//	can_fifo_push(CANMessage.ulMsgID, CANMessage.ulMsgLen, ucMsgData);
	can_data_read(CANMessage.ulMsgID, CANMessage.ulMsgLen, ucMsgData);
}

void CAN0_Handler()
{
	unsigned long ulStatus;

	//
	// Read the CAN interrupt status to find the cause of the interrupt
	//
	ulStatus = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

	//
	// If the cause is a controller status interrupt, then get the status
	//
	if(ulStatus == CAN_INT_INTID_STATUS)
	{
		//
		// Read the controller status.  This will return a field of status
		// error bits that can indicate various errors.  Error processing
		// is not done in this example for simplicity.  Refer to the
		// API documentation for details about the error status bits.
		// The act of reading this status will clear the interrupt.
		//
		ulStatus = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
	}

	//
	// Check if the cause is message object 1.
	//
	else if(ulStatus == 1)
	{
		CAN_GetMessage(CAN0_BASE, 1);
		//
		// Getting to this point means that the RX interrupt occurred on
		// message object 1, and the message reception is complete.  Clear the
		// message object interrupt.
		//
		CANIntClear(CAN0_BASE, 1);
	}
	
	//
	// Otherwise, something unexpected caused the interrupt.  This should
	// never happen.
	//
	else
	{
		//
		// Spurious interrupt handling can go here.
		//
	}
}

