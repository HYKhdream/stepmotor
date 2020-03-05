/*
 * arch_uart.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f37x.h"
#include "config.h"

#define COM_MAX                     1

#define COM1_DEV                    USART1
#define COM1_CLK                    RCC_APB2Periph_USART1

#define COM1_TX_PIN                 GPIO_Pin_9
#define COM1_TX_GPIO_PORT           GPIOA
#define COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define COM1_TX_SOURCE              GPIO_PinSource9
#define COM1_TX_AF                  GPIO_AF_7

#define COM1_RX_PIN                 GPIO_Pin_10
#define COM1_RX_GPIO_PORT           GPIOA
#define COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define COM1_RX_SOURCE              GPIO_PinSource10
#define COM1_RX_AF                  GPIO_AF_7

#define COM1_IRQn                   USART1_IRQn


USART_TypeDef* COM_USART[COM_MAX] = {COM1_DEV}; 

GPIO_TypeDef* COM_TX_PORT[COM_MAX] = {COM1_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COM_MAX] = {COM1_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COM_MAX] = {COM1_CLK};

const uint32_t COM_TX_PORT_CLK[COM_MAX] = {COM1_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COM_MAX] = {COM1_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COM_MAX] = {COM1_TX_PIN};

const uint16_t COM_RX_PIN[COM_MAX] = {COM1_RX_PIN};
 
const uint16_t COM_TX_PIN_SOURCE[COM_MAX] = {COM1_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COM_MAX] = {COM1_RX_SOURCE};
 
const uint8_t COM_TX_AF[COM_MAX] = {COM1_TX_AF};
 
const uint8_t COM_RX_AF[COM_MAX] = {COM1_RX_AF};



typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;


/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);

	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

	/* Configure USART Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
	GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

	/* USART configuration */
	USART_Init(COM_USART[COM], USART_InitStruct);

	/* Enable USART */
	USART_Cmd(COM_USART[COM], ENABLE);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = COM1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void UART_config()
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_Config();

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	STM_COMInit(COM1, &USART_InitStructure);

	//USART_ITConfig(COM1_DEV, USART_IT_TXE, ENABLE);
	USART_ITConfig(COM1_DEV, USART_IT_RXNE, ENABLE);
}

#define BUFF_SIZE	1000
int uart_tx_buff[BUFF_SIZE];
int uart_tx_rptr = 0;
int uart_tx_wptr = 0;

int uart_tx_buff_count()
{
	int count = BUFF_SIZE + uart_tx_wptr - uart_tx_rptr;
	if(count >= BUFF_SIZE)
		count -= BUFF_SIZE;

	return count;
}

int uart_tx_buff_push(int ch)
{
	if(uart_tx_buff_count() >= BUFF_SIZE - 1)
		return -1;
	uart_tx_buff[uart_tx_wptr] = ch;
	uart_tx_wptr++;
	if(uart_tx_wptr >= BUFF_SIZE) {
		uart_tx_wptr -= BUFF_SIZE;
	}

	return 0;
}

int uart_tx_buff_pop()
{
	int ch;

	if(uart_tx_buff_count() == 0)
		return 0;

	ch = uart_tx_buff[uart_tx_rptr];
	uart_tx_rptr++;
	if(uart_tx_rptr >= BUFF_SIZE) {
		uart_tx_rptr -= BUFF_SIZE;
	}

	return ch;
}

int uart_ximt_char(int ch)
{
	//USART_SendData(COM1_DEV, ch);
	/* Loop until transmit data register is empty */
	//while (USART_GetFlagStatus(COM1_DEV, USART_FLAG_TXE) == RESET) {}
	uart_tx_buff_push(ch);
	if(ch == '\n') {
		uart_tx_buff_push('\r');
	}
	USART_ITConfig(COM1_DEV, USART_IT_TXE, ENABLE);
}

void Shell_PrintChar(char ch)
{
	uart_ximt_char(ch);
}

void UART_isr()
{
	if(USART_GetITStatus(COM1_DEV, USART_IT_RXNE) != RESET)
	{
	#ifdef USE_SHELL
		void shell_push_char(char ch);
		int RxBuffer;
		RxBuffer = (USART_ReceiveData(COM1_DEV) & 0x7F);
		shell_push_char(RxBuffer);
	#endif	
	}

	if(USART_GetITStatus(COM1_DEV, USART_IT_TXE) != RESET)
	{
		USART_SendData(COM1_DEV, uart_tx_buff_pop());
		if(uart_tx_buff_count() <= 0) {
			/* Disable the COM1_DEV Transmit interrupt */
			USART_ITConfig(COM1_DEV, USART_IT_TXE, DISABLE);
		}
	}
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	uart_ximt_char(ch);
	return ch;
}


