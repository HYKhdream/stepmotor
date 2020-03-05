#ifndef __ARCH_UART_H_
#define __ARCH_UART_H_
#include "stdint.h"

void UART0_Handler();
/* Public functions ----------------------------------------------------------*/
int arch_uart_config(void);
void arch_uart_putc(uint8_t ch);
int arch_uart_getc(uint8_t *buf);//读取指定长度数据

#endif