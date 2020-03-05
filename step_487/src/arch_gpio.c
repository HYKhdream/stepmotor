/*
 * arch_adc.c	(by xhl 2014/10/31)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "stm32f37x.h"

/*
 *
 * Input:
 *	PA0->?,	PA1->IN1, PA2->ADC_IN2_24V, PA3->ADC_IN3_12V
 *	PC0->IN8, PC1->IN4, PC2->IN3, PC3->IN2, PC6..9->ID0..3,
 *	PC13->IN5, PC14->IN6, PC15->IN7
 *
 * Output:
 *	PA4->DAC_MOTOR, PA5..7->STEP1..3, PA15->STEP5
 *	PB0->TQ1, PB1->DIR2, PB2->TQ2, PB3->STEP7, PB4->TQ6, 
 *	PB5->DIR7, PB6->TQ7, PB7->STEP8, PB8->DIR8,
 *	PB9->TQ8, PB14->DIR4, PB15->TQ4
 *	PC4->STEP4, PC5->DIR1, PC10->DIR5, PC11->STEP6,
 *	PC12->TQ5
 *	PD2->DIR6, PD8->LED
 *	PE8->DIR3, PE9->TQ3
 *	PF6-<MOTOREN, PF7->MOTORRESET
 * 
 * Misc
 *	PA11..12->CAN
 */

void GPIO_config()
{
	uint32_t cfg_reg;

	GPIO_InitTypeDef GPIO_InitStructure;

	cfg_reg = RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC;
	cfg_reg |= RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF;
	RCC_AHBPeriphClockCmd(cfg_reg, ENABLE);

	/* Input */

	cfg_reg = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Pin = cfg_reg;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	cfg_reg = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	cfg_reg |= GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	cfg_reg |= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Pin = cfg_reg;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Output */

	cfg_reg = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_0 |GPIO_Pin_8 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Pin = cfg_reg;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	cfg_reg = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	cfg_reg |= GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	cfg_reg |= GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Pin = cfg_reg;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	cfg_reg = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Pin = cfg_reg;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_8/*RUN LED*/;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	

	
}


#define FULL	0
#define HALF	1
#define QUARTER	2
#define EIGHTH	3



void Set_stepmotor_sub(int sub)
{
	switch (sub)
	{
		case FULL:
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, 1);
			GPIO_WriteBit(GPIOA, GPIO_Pin_8, 0);
		}
		break;
		case HALF:
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, 1);
			GPIO_WriteBit(GPIOA, GPIO_Pin_8, 1);
		}
		break;
		case QUARTER:
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, 0);
			GPIO_WriteBit(GPIOA, GPIO_Pin_8, 0);
		}
		break;

		case EIGHTH:
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, 0);
			GPIO_WriteBit(GPIOA, GPIO_Pin_8, 1);
		}
		break;
	}
}

extern int Get_doard_type_ex();



unsigned int Triangle_is_left_Sign_ex(unsigned int stepno,unsigned int workidx)
{
		if (!Get_doard_type_ex())
			{
		if (!arch_input_check(stepno))
			{
		return 1;}
		else
			{
		return 0;}
			
			}
		else
			{
				
						if ((!arch_input_check(stepno))&&(arch_input_check(workidx)))
						{
							return 1;}
						else
							{
							return 0;}
					
			}
			
		
}


unsigned int Triangle_is_left_Sign(unsigned int stepno)
{
	if (!Get_doard_type_ex())
			{
		if (!arch_input_check(stepno))
			{
		return 1;}
		else
			{
		return 0;}
			
		}
	else
		{
			
				if ((!arch_input_check(stepno))&&(arch_input_check(stepno+4)))
					{
				return 1;}
				else
					{
				return 0;}
			
		}		
}

unsigned int Triangle_is_right_Sign_ex(unsigned int stepno,unsigned int workidx)
{
	if (!Get_doard_type_ex())
			{
		if (arch_input_check(stepno))
			{
		return 1;}
		else
			{
		return 0;}
			
		}
	else
		{
		if ((arch_input_check(stepno))&&(!arch_input_check(workidx)))
			{
		return 1;}
		else
			{
		return 0;}	
		}
}

unsigned int Triangle_is_right_Sign(unsigned int stepno)
{
	if (!Get_doard_type_ex())
	{
		if (arch_input_check(stepno))
		{
			return 1;
		}
		else
		{
			return 0;
		}
			
	}else
	{
		if ((arch_input_check(stepno))&&(!arch_input_check(stepno+4)))
			{
		return 1;}
		else
			{
		return 0;}	
	}
}


