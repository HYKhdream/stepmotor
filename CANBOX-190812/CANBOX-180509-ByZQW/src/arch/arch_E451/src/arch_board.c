/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/interrupt.h"
#include "inc/sysctl.h"
#include "inc/gpio.h"
#include "inc/timer.h"
#include "inc/ssi.h"
#include "inc/can.h"
#include "inc/eeprom.h"
#include "inc/uart.h"

#include "arch_ra8875.h"
#include "arch_board.h"
#include "arch_uart.h"
#include "canfifo.h"

void arch_touch_timer(void);
static int __arch_ms = 0;
static int __arch_can_delay = 0;
static unsigned long timer_ticks = 0;

void arch_gpio_config()
{
	// Setup SSI0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	GPIOPinConfigure(GPIO_PA4_SSI0RX);
	GPIOPinConfigure(GPIO_PA5_SSI0TX);
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	// Setup GPIOD3 is RUNLED
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
	// Setup GPIOD2 is BackLight
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	// Setup GPIOF0 is RA8875 Busy
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	// Setup GPIOF1 is RA8875 reset
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	// Setup GPIOA3 is RA8875 CS
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
}

int arch_can_config()
{
	tCANMsgObject CANMessage;
	unsigned long ratio;

	ratio = can_bps_get();
	ratio *= 1000;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinConfigure(GPIO_PE4_CAN0RX);
	GPIOPinConfigure(GPIO_PE5_CAN0TX);

	GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
	CANInit(CAN0_BASE);
	
	CANBitRateSet(CAN0_BASE, SysCtlClockGet(), ratio);
	//CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 1000000);
	//CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER/*|CAN_INT_STATUS|CAN_INT_ERROR*/);

	IntEnable(INT_CAN0);

	CANEnable(CAN0_BASE);

	IntPrioritySet(INT_CAN0, 0);

	IntMasterEnable();
	//
	// Initialize a message object to be used for receiving CAN messages with
	// any CAN ID.  In order to receive any CAN ID, the ID and mask must both
	// be set to 0, and the ID filter enabled.
	//
	CANMessage.ulMsgID = 0;                        // CAN msg ID - 0 for any
	CANMessage.ulMsgIDMask = 0;                    // mask is 0 for any ID
	CANMessage.ulFlags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
	CANMessage.ulMsgLen = 8;                       // allow up to 8 bytes

	//
	// Now load the message object into the CAN peripheral.  Once loaded the
	// CAN will receive any message on the bus, and an interrupt will occur.
	// Use message object 1 for receiving messages (this is not the same as
	// the CAN ID which can be any value in this example).
	//
	CANMessageSet(CAN0_BASE, 1, &CANMessage, MSG_OBJ_TYPE_RX);

	return 0;
}


void arch_can_send(unsigned long MsgID, int len, unsigned char *data)
{
	tCANMsgObject CANMessage_Send;

	CANMessage_Send.ulMsgID = MsgID;
	CANMessage_Send.ulMsgLen = len;
	CANMessage_Send.ulMsgIDMask = 0;
	CANMessage_Send.pucMsgData = data;
	CANMessage_Send.ulFlags = /*MSG_OBJ_TX_INT_ENABLE |*/ MSG_OBJ_USE_ID_FILTER;

	CANMessageSet(CAN0_BASE, 15, &CANMessage_Send, MSG_OBJ_TYPE_TX);
}

void arch_spi_config()
{
	// Enable SSI0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);

	SSIEnable(SSI0_BASE);
}

void arch_spi_config_HS()
{
	// Enable SSI0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 8000000, 8);

	SSIEnable(SSI0_BASE);
}

int arch_SPI_xfer(int data)
{
	unsigned long rdata;

	SSIDataPut(SSI0_BASE, data);

	SSIDataGet(SSI0_BASE, &rdata);

	return rdata;
}

void arch_eeprom_config()
{
	unsigned long lret;
	unsigned long rDat[10];

	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	lret = EEPROMInit();
	//lret = EEPROMSizeGet();
	//lret = EEPROMBlockCountGet();
}

void arch_eeprom_read(void *pDat, int addr, int count)
{
	EEPROMRead(pDat, addr, count);
}

void arch_eeprom_write(void *pDat, int addr, int count)
{
	EEPROMProgram(pDat, addr, count);
}
void Timer1B_Handler_delay(void);
void Timer2B_Handler(void);
//{
//	TimerIntClear(WTIMER1_BASE, TIMER_TIMB_TIMEOUT);
//	can_send_cont();
//}
void arch_timer0_init()
{
	//
	
	// enable timer0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	// initialize the work mode of the TIMER0A
	TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PERIODIC);

	//configure to 1ms
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 10000);

	IntPrioritySet(INT_TIMER0A, 3);

	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	IntEnable(INT_TIMER0A);

	TimerEnable(TIMER0_BASE, TIMER_A);
}

void arch_timer1_init()
{
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);


	TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR |TIMER_CFG_B_PERIODIC);

	TimerLoadSet(WTIMER1_BASE, TIMER_B, SysCtlClockGet()/10000);

	TimerIntRegister(WTIMER1_BASE, TIMER_B, Timer1B_Handler_delay);
	
	IntMasterEnable();
	
	TimerIntEnable(WTIMER1_BASE, TIMER_TIMB_TIMEOUT);

	IntEnable(INT_WTIMER1B);

	TimerEnable(WTIMER1_BASE, TIMER_B);
}

void arch_timer2_init()
{
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);


	TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR |TIMER_CFG_B_PERIODIC);

	TimerLoadSet(WTIMER2_BASE, TIMER_B, SysCtlClockGet()/4000);

	TimerIntRegister(WTIMER2_BASE, TIMER_B, Timer2B_Handler);
	
	IntMasterEnable();
	
	TimerIntEnable(WTIMER2_BASE, TIMER_TIMB_TIMEOUT);

	IntEnable(INT_WTIMER2B);

	TimerEnable(WTIMER2_BASE, TIMER_B);
}

int arch_runled_setup(int onoff)
{
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, onoff ? GPIO_PIN_3 : 0x00);

	return 0;
}

void arch_runled_timer()
{
	static int led_delay = 0;
	static int led_status = 0;
	led_delay++;

	if(led_delay > 1000) {
		led_delay = 0;
		arch_runled_setup(led_status);
		led_status = !led_status;
	}
}

int arch_BackLight_GetMode()
{
	static int backlight_mode = 0;

	if(backlight_mode)
		return backlight_mode & 0x1;

	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_2);
	if(GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_2)) {
		backlight_mode = 0x1;
	}
	backlight_mode |= 0x80;

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_2);

	return backlight_mode & 0x1;
}

void arch_BackLight_Onoff(int onoff)
{
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, onoff ? GPIO_PIN_2 : 0);
}

int arch_babklight_bright_setup = 0;
int arch_babklight_bright_ticks = 0;
void arch_BackLight_setup(int _bright)
{
	if(arch_BackLight_GetMode()) {
		if(_bright == 0 || _bright >= 100) {
			arch_BackLight_Onoff(_bright);
			arch_babklight_bright_setup = 0;
		}
		else {
			arch_babklight_bright_setup = _bright;
		}
		return ;
	}

	arch_babklight_bright_setup = 0;
	RA8875_SetBackLight(_bright * 255L / 100L);
}

void arch_BackLight_timer()
{
	if(!arch_babklight_bright_setup)
		return ;
	arch_babklight_bright_ticks++;
	if(arch_babklight_bright_ticks >= 100) {
		arch_babklight_bright_ticks = 0;
		arch_BackLight_Onoff(1);
	}
	if(arch_babklight_bright_ticks == arch_babklight_bright_setup) {
		arch_BackLight_Onoff(0);
	}
}

void arch_RA8875_Reset_setup(int onoff)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, (onoff) ? GPIO_PIN_1 : 0x00);
}

void arch_RA8875_CS_setup(int onoff)
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, (onoff) ? GPIO_PIN_3 : 0x00);
}

int arch_RA8875_is_busy()
{
	return (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0) == 0);
}

void arch_delay_ms(int _ms)
{
	__arch_ms = _ms;
	while(__arch_ms > 0);
}

int arch_can_delay_nx100us(unsigned char n)
{
	if(__arch_can_delay>=n)
	{
		__arch_can_delay =0;
		return 1;
	}
	else return 0;
}

void arch_hook_100us()
{
	static int delay_ticks = 0;

	timer_ticks ++;
	arch_BackLight_timer();
	do_filter();

	if(__arch_can_delay<100)
	{
		__arch_can_delay++;
	}

	if(++delay_ticks < 10) {
		return;
	}
	delay_ticks = 0;

	arch_runled_timer();
	arch_touch_timer();
	
	if(__arch_ms > 0) __arch_ms--;
}

unsigned long arch_get_systicks()
{
	return timer_ticks;
}

int arch_init()
{
	//
	// Set the clocking to run at 20 MHz (200 MHz / 10) using the PLL.  When
	// using the ADC, you must either use the PLL or supply a 16 MHz clock
	// source.
	// TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
	// crystal on your board.
	//
	// Set the clocking to run at 25 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_8MHZ);

	arch_gpio_config();

	arch_eeprom_config();
	
	arch_can_config();
	arch_uart_config();
	//arch_BackLight_setup(0);
	arch_spi_config();
	
	arch_timer0_init();
	arch_timer1_init();
	arch_timer2_init();
	RA8875_Init();
	arch_spi_config_HS();
	arch_BackLight_setup(255);

	// 不能去，去掉触摸屏工作异常
	//RA8875_Test();
	return 0;
}

