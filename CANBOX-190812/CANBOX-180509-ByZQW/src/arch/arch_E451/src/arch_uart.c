/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "inc/interrupt.h"
#include "inc/sysctl.h"
#include "inc/gpio.h"
#include "inc/uart.h"
#include "arch_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UART_RX_BUFF_SIZE		64

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t uart_rx_fifo[UART_RX_BUFF_SIZE];
static uint8_t uart_rx_read = 0;
static uint8_t uart_rx_write = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
unsigned char rcbuf;
void UART0_Handler()
{
	unsigned long ulStatus;
	

	// Get the interrrupt status.
	ulStatus = UARTIntStatus(UART0_BASE, true);
	// Clear the asserted interrupts.
	UARTIntClear(UART0_BASE, ulStatus);
	// Loop while there are characters in the receive FIFO.
	while (UARTCharsAvail(UART0_BASE))
	{
		
		rcbuf = UARTCharGetNonBlocking(UART0_BASE);
		uart_rx_fifo[uart_rx_write] =rcbuf;
		uart_rx_write++;
		if (uart_rx_write >= UART_RX_BUFF_SIZE)
		{
			uart_rx_write = 0;
		}
	}
}

/* Public functions ----------------------------------------------------------*/

int arch_uart_config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 38400, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_TWO | UART_CONFIG_PAR_NONE);
	
	IntMasterEnable();
	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	return 0;
}

void arch_uart_putc(uint8_t ch)
{
	UARTCharPut(UART0_BASE, ch);
}

/*
	读取指定长度数据
*/
int arch_uart_getc(uint8_t *buf)
{
    if (uart_rx_write == uart_rx_read)
		return 0;
	*buf = uart_rx_fifo[uart_rx_read];
	uart_rx_read++;
	if (uart_rx_read >= UART_RX_BUFF_SIZE)
		uart_rx_read = 0;

	return 1;
}

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	arch_uart_putc(ch);

	return ch;
}

