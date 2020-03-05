#ifndef __ARCH_BOARD_H_
#define __ARCH_BOARD_H_

void arch_gpio_config(void);
int arch_can_config(void);
void arch_can_send(unsigned long MsgID, int len, unsigned char *data);
void arch_spi_config(void);
void arch_spi_config_HS(void);
int arch_SPI_xfer(int data);
void arch_eeprom_config(void);
void arch_eeprom_read(void *pDat, int addr, int count);
void arch_eeprom_write(void *pDat, int addr, int count);
void Timer1B_Handler_delay(void);
void Timer2B_Handler(void);
void arch_timer0_init(void);
void arch_timer1_init(void);
void arch_timer2_init(void);
int arch_runled_setup(int onoff);
void arch_runled_timer(void);
int arch_BackLight_GetMode(void);
void arch_BackLight_Onoff(int onoff);
void arch_BackLight_timer(void);
void arch_RA8875_CS_setup(int onoff);
void arch_hook_100us(void);
int arch_init(void);
void arch_delay_ms(int _ms);


#endif