/*
 * arch_board.c	(by xhl 2014/11/03)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#include "stm32f37x.h"

#define YARN_STEP_CAN_RX_MAILBOX_ID (0x370)
#define TRIANGLE_CAN_RX_MAILBOX_ID   (0x330)



#ifdef TRIANGLE_STEP_SUPPORT
extern volatile unsigned int Triangle_mode; 	
#endif

 
volatile unsigned int Board_isE487;

int BootVer[4] __attribute__((at(0x20003000)));
volatile  int BootVer_d;



volatile unsigned short board_id=0; 

volatile unsigned short board_type=0; 

volatile unsigned short board_type_ex=0; 


void GPIO_config(void);
void UART_config(void);
void CAN_Config(int GET_ID);
void ADC_config(void);
void DAC_Config(void);
void TIMER_Config(void);

unsigned int arch_input_f7();
unsigned int arch_check_isE487();




void ReBoot(void)
{
#if 0
	JumpAddress = *(volatile uint32_t*) (BOOT_START_ADDRESS + 4);
	Jump_To = (pFunction) JumpAddress;
	/* Initialize user application's Stack Pointer */
	__MSR_MSP(*(volatile uint32_t*) BOOT_START_ADDRESS);
	Jump_To();
#else
	//NVIC_SETFAULTMASK();
	//NVIC_GenerateSystemReset();
NVIC_SystemReset();
	//NVIC_GenerateCoreReset();
#endif
}



void Upgrade_Start(void)
{
#define CMD_UPGRADE_START	0xA050AA00
#define CMD_UPGRADE_RCVDATA	0xA050AA55
#define CMD_UPGRADE_BURN	0xA0505A5A
#define CMD_UPGRADE_SUCCESS	0xA05055AA
	u32 upgrade_cmd = CMD_UPGRADE_START;
	EE_Write(0, (u16*)&upgrade_cmd, 2);

	//myprintf("upgrade start \n\r");

	Delay(10);

	ReBoot();
}



void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_ACR_PRFTBE);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_CFGR_PLLSRC_PREDIV1, RCC_PLLMul_9);

		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while((RCC_GetSYSCLKSource() >> 2) != RCC_SYSCLKSource_PLLCLK)
		{
		}
	}

	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
}


void arch_init()
{
	
}


int arch_dip_input()
{
	int sts = 0;
	sts = ((~ GPIO_ReadInputData(GPIOC) ) & 0x03f0)>>6 ; 
	return sts;

}

static unsigned int arch_input_f7()
{
	return  !(GPIO_ReadInputData(GPIOF) & (0x0001<<7));  
	
}
	
int arch_input_check(int idx)
{
	int sts = 0;
	unsigned int id=0;

	do
	{
	id++;
	switch(idx) {
	case 0:	// A1
		sts = !(GPIO_ReadInputData(GPIOA) & 0x0002);
		break;
	case 1: // C3
		sts = !(GPIO_ReadInputData(GPIOC) & 0x0008);
		break;
	case 2: // C2
		sts = !(GPIO_ReadInputData(GPIOC) & 0x0004);
		break;
	case 3: // C1
		sts = !(GPIO_ReadInputData(GPIOC) & 0x0002);
		break;
	case 4: // C13
		sts = !(GPIO_ReadInputData(GPIOC) & 0x2000);
		break;
	case 5: // C14
		sts = !(GPIO_ReadInputData(GPIOC) & 0x4000);
		break;
	case 6: // C15
		sts = !(GPIO_ReadInputData(GPIOC) & 0x8000);
		break;
	case 7: // C0
		sts = !(GPIO_ReadInputData(GPIOC) & 0x0001);
		break;
	}
		}while((sts==0)&&(id<3));
	return sts;
}

int arch_get_zero_status()
{
	int arch_input_check(int idx);
	int sts = 0;
	int i;
	for(i = 0; i < 8; i++) {
		if(arch_input_check(i)) {
			sts |= 1L << i;
		}
	}

	return sts;
}

int arch_input_boardid()
{
	return (GPIO_ReadInputData(GPIOC) >> 6) & 0xF;
}

void arch_run_led_toggle()
{
	GPIOD->ODR ^= GPIO_Pin_8;
}

void arch_step_motro_reset()
{
	int i = 100;
	GPIOF->ODR |= GPIO_Pin_7;
	while(--i);
	GPIOF->ODR &= ~GPIO_Pin_7;
}

void arch_step_motro_en(int onoff)
{
	GPIO_WriteBit(GPIOF, GPIO_Pin_6, onoff);
}

int arch_get_step_sign_()
{
	unsigned int ret = 0;
	int i;
	for (i=0;i<8;i++)
	{
		ret |= ((!arch_input_check(i))<<i);
	}

	return ret;


}
extern volatile unsigned int EMF_Operate_Reback;


int arch_stepMotor_inout_sts(int idx1)
{
	return arch_input_check(idx1) ;

}

int arch_StepMotor_Zero_EX(int idx1,int idx2)
{

#ifdef TEST_DEBUG_CODE_ENABLE
	if(EMF_Operate_Reback)			//test mode 
	{
		return arch_input_check(idx1);
	}
	else
		{	

#endif


if (Is_Yarn_Board() )
{
	return arch_input_check(idx1);
}
else
{
	if (Get_doard_type_ex())
	{
		#ifdef TRIANGLE_STEP_SUPPORT
			if(Triangle_mode)
				return arch_input_check(idx1) ;
			else
				return arch_input_check(idx1) && arch_input_check(idx2);	
		#else
			
		return arch_input_check(idx1) && arch_input_check(idx2);	
		#endif
	}	
	else
	{
		return arch_input_check(idx1);
	}
}
#ifdef TEST_DEBUG_CODE_ENABLE
		}
#endif
}

int arch_StepMotor_Zero(int idx)
{
#ifdef TEST_DEBUG_CODE_ENABLE
	if(EMF_Operate_Reback)			//test mode 
	{
		return arch_input_check(idx);
	}
	else
		{	

#endif


if (Is_Yarn_Board())
{return arch_input_check(idx);}
else
{
	if (Get_doard_type_ex())
	{
		#ifdef TRIANGLE_STEP_SUPPORT
			if(Triangle_mode)
				return arch_input_check(idx) ;
			else
				return arch_input_check(idx) && arch_input_check(idx+4);	
		#else
			
		return arch_input_check(idx) && arch_input_check(idx+4);	
		#endif		
	}
	else
	{
		return arch_input_check(idx);
	}
}
#ifdef TEST_DEBUG_CODE_ENABLE
		}
#endif
}



void arch_StepMotor_Half(int stepno, int onoff)
{
	switch(stepno) {
	case 0:
		GPIO_WriteBit(GPIOB, GPIO_Pin_0, !onoff);
		break;
	case 1:
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, !onoff);
		break;
	case 2:
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, !onoff);
		break;
	case 3:
		GPIO_WriteBit(GPIOB, GPIO_Pin_15, !onoff);
		break;
	case 4:
		GPIO_WriteBit(GPIOC, GPIO_Pin_12, !onoff);
		break;
	case 5:
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, !onoff);
		break;
	case 6:
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, !onoff);
		break;
	case 7:
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, !onoff);
		break;
	}
}

void arch_StepMotor_Dir(int stepno, int dir)
{
			
	switch(stepno) {
	case 0:
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, !dir);
		break;
	case 1:
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, !dir);
		break;
	case 2:
		GPIO_WriteBit(GPIOE, GPIO_Pin_8, !dir);
		break;
	case 3:
		GPIO_WriteBit(GPIOB, GPIO_Pin_14, !dir);
		break;
	case 4:
		GPIO_WriteBit(GPIOC, GPIO_Pin_10, !dir);
		break;
	case 5:
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, !dir);
		break;
	case 6:
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, !dir);
		break;
	case 7:
		GPIO_WriteBit(GPIOB, GPIO_Pin_8, !dir);
		break;
	}
}


void arch_StepMotor_Pulse(int stepno, int onoff)
{
	switch(stepno) {
	case 0:		 
		if (arch_check_isE487())
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_8, onoff);
		}
		else		
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, onoff);
		break;
	case 1:
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, onoff);
		break;
	case 2:
		GPIO_WriteBit(GPIOA, GPIO_Pin_7, onoff);
		break;
	case 3:
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, onoff);
		break;
	case 4:
		GPIO_WriteBit(GPIOA, GPIO_Pin_15, onoff);
		break;
	case 5:
		GPIO_WriteBit(GPIOC, GPIO_Pin_11, onoff);
		break;
	case 6:
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, onoff);
		break;
	case 7:
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, onoff);
		break;
	}
}

void TIMER_SetCompare1(TIM_TypeDef* TIMx, unsigned long capture);
void TIMER_SetCompare2(TIM_TypeDef* TIMx, unsigned long capture);
void TIMER_SetCompare3(TIM_TypeDef* TIMx, unsigned long capture);
void TIMER_SetCompare4(TIM_TypeDef* TIMx, unsigned long capture);
void arch_StepMotor_SetCompare(unsigned int stepno, int capture)
{
	switch(stepno) {
	case 0:
		TIMER_SetCompare1(TIM14, capture);
		break;
	case 1:
		TIMER_SetCompare1(TIM3, capture);
		break;
	case 2:
		TIMER_SetCompare2(TIM3, capture);
		break;
	case 3:
		TIMER_SetCompare1(TIM13, capture);
		break;
	case 4:
		TIMER_SetCompare2(TIM19, capture);
		break;
	case 5:
		TIMER_SetCompare2(TIM12, capture);
		break;
	case 7:
		TIMER_SetCompare2(TIM2, capture);
		break;
	case 6:
		TIMER_SetCompare4(TIM3, capture);
		break;
	}
}

volatile unsigned long step_speed[10];

void arch_StepMotor_Set_Speed(unsigned int stepno, unsigned int speed)
{
	step_speed[stepno] = speed;
}

void arch_StepMotor_Start(unsigned int stepno)
{
	arch_StepMotor_SetCompare(stepno, 100);
}

void arch_StepMotor_Active(unsigned int stepno,unsigned int step_speed)
{
	unsigned long capture = step_speed;
	extern volatile unsigned int step_timer_cnt;

	capture += step_timer_cnt;
	arch_StepMotor_SetCompare(stepno, capture&0xFFFF);
}

void arch_StepMotor_Stop(unsigned int stepno)
{
	switch(stepno) {
	case 0:
		TIM_ITConfig(TIM14, TIM_IT_CC1, DISABLE);
		break;
	case 1:
		TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
		break;
	case 2:
		TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
		break;
	case 3:
		TIM_ITConfig(TIM13, TIM_IT_CC1, DISABLE);
		break;
	case 4:
		TIM_ITConfig(TIM19, TIM_IT_CC2, DISABLE);
		break;
	case 5:
		TIM_ITConfig(TIM12, TIM_IT_CC2, DISABLE);
		break;
	case 7:
		TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
		break;
	case 6:
		TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);
		break;
	}
}



void arch_StepMotor_Init()
{
	int i;

	for(i = 0; i < 8; i ++) {
		arch_StepMotor_Half(i, 0);
		arch_StepMotor_Dir(i, 0);
		arch_StepMotor_Pulse(i, 0);
	}
	
}

void arch_StepMotor_Enable(void)
{
	// by xhl 2012/08/07
	//void arch_StepMotor_Init();
	arch_StepMotor_Init();

	arch_step_motro_en(0x1);
}

void arch_StepMotor_Disable(void)
{
	arch_step_motro_en(0);
}



int Get_board_id()
{
	return board_id;
}

int Is_Yarn_Board()
{
	return board_type?1:0;
}

int Get_doard_type()
{
	return board_type;
}

int Get_doard_type_ex()/*是否支持两个传感器*/
{
	return board_type_ex;
}





/*读取板子的boot程序版本信息*/
static void arch_set_board_bootver()
{
	BootVer_d =0;	
	if ((BootVer[0] ==  0x00000001) &&(BootVer[1] ==  0x00000002) &&(BootVer[2] ==  0x00000003))
	{
		if (BootVer[3] ==  (BootVer[0]+BootVer[1]+BootVer[2]))
			BootVer_d =1;	
	}	
}

unsigned int arch_get_board_bootver()
{
	return BootVer_d;
}

void Init_core()
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x7000);
	RCC_Configuration();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	arch_set_board_bootver();
}

/*读取板子的拨码开关信息*/
static  void arch_set_board_DIP()
{
	unsigned short Dip_data;
	Dip_data = arch_dip_input();
	board_type = Dip_data >>3;
	board_type_ex = (Dip_data >>2) & 0x01 ;	
	board_id= Dip_data & 0x03; 

}

/*读取板子的特定管脚表示是否是E487板*/
static void arch_set_board_isE487()
{
	Board_isE487 = arch_input_f7()?0:1;
}


unsigned int arch_check_isE487()
{
	return Board_isE487;
}

void Init_hardware_first()
{
	GPIO_config();
	arch_set_board_isE487();
	arch_set_board_DIP();
}

void Init_hardware()
{	
	
	TIMER_Config();
	ADC_config();	
	
	
	CAN_Config((Is_Yarn_Board() ? YARN_STEP_CAN_RX_MAILBOX_ID :TRIANGLE_CAN_RX_MAILBOX_ID)+Get_board_id() );
	
	if (arch_check_isE487())
	{
		DAC12_Config();
	}
	else	
	{
		DAC_Config();
	}
	FLASH_Unlock();
	EE_Init();
}


void Init_parameter()
{
	alert_init();
	Massage_Init();
	StepMotor_Init();
}

void Init_output()
{

	StepMotor_init_output();
	

}

