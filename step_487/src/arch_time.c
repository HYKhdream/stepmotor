/*
 * arch_time.c	(by xhl 2014/11/1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Config.h"
#include "stm32f37x.h"

volatile uint16_t CCR1_Val = 40960;
volatile uint16_t CCR2_Val = 20480;
volatile uint16_t CCR3_Val = 10240;
volatile uint16_t CCR4_Val = 5120;


#define TIM3_USE
static void TIMS_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* TIM clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
#ifdef TIM3_USE
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
#else
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM19, ENABLE);

#if 0
	/* GPIO clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect TIM Channels to AF1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_9);
#ifdef TIM3_USE
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_2);
#else
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
#endif
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_10);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1);
#ifdef TIM3_USE
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_10);
#else
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_9);
#endif
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_2);
#endif

	/* Enable the TIM global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM12_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM13_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_Init(&NVIC_InitStructure);

#ifdef TIM3_USE
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
#else
	NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
	NVIC_Init(&NVIC_InitStructure);
#endif

	NVIC_InitStructure.NVIC_IRQChannel = TIM19_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1/*0*/;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;


	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
#ifdef TIM3_USE
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
#else
	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
#endif
	TIM_TimeBaseInit(TIM19, &TIM_TimeBaseStructure);

	/* Output Compare Toggle Mode configuration: Channel1 */
	//TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 150;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OC1Init(TIM13, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Disable);
#ifdef TIM3_USE
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
#else
	TIM_OC1Init(TIM16, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Disable);
	TIM_OC1Init(TIM17, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Disable);
#endif

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Disable);
#ifndef TIM3_USE
	TIM_OC2Init(TIM15, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM15, TIM_OCPreload_Disable);
#endif
	TIM_OC2Init(TIM19, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM19, TIM_OCPreload_Disable);

	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM12, ENABLE);
	TIM_Cmd(TIM13, ENABLE);
	TIM_Cmd(TIM14, ENABLE);
#ifdef TIM3_USE
	TIM_Cmd(TIM3, ENABLE);
#else
	TIM_Cmd(TIM15, ENABLE);
	TIM_Cmd(TIM16, ENABLE);
	TIM_Cmd(TIM17, ENABLE);
#endif
	TIM_Cmd(TIM19, ENABLE);

#if 0
	TIM_ITConfig(TIM13, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);
#ifdef TIM3_USE
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC4, ENABLE);
#else
	TIM_ITConfig(TIM16, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM17, TIM_IT_CC1, ENABLE);
#endif

	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM12, TIM_IT_CC2, ENABLE);
#ifndef TIM3_USE
	TIM_ITConfig(TIM15, TIM_IT_CC2, ENABLE);
#endif
	TIM_ITConfig(TIM19, TIM_IT_CC2, ENABLE);
#endif
	//TIM_ITConfig(TIM3, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);
	//TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);

	
} 

void TIMER_Config(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	/* Setup SysTick Timer for 1 msec interrupts.
	   ------------------------------------------
	  1. The SysTick_Config() function is a CMSIS function which configure:
		 - The SysTick Reload register with value passed as function parameter.
		 - Configure the SysTick IRQ priority to the lowest value (0x0F).
		 - Reset the SysTick Counter register.
		 - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
		 - Enable the SysTick Interrupt.
		 - Start the SysTick Counter.

	  2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
		 SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
		 SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
		 inside the misc.c file.

	  3. You can change the SysTick IRQ priority by calling the
		 NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
		 call. The NVIC_SetPriority() is defined inside the core_cm4.h file.

	  4. To adjust the SysTick time base, use the following formula:

		   Reload Value = SysTick Counter Clock (Hz) x	Desired Time base (s)

		 - Reload Value is the parameter to be passed for SysTick_Config() function
		 - Reload Value should not exceed 0xFFFFFF
	 */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	TIMS_Config();
}

void TIMER_SetCompare1(TIM_TypeDef* TIMx, unsigned long capture)
{
	capture += TIM_GetCounter(TIMx);
	TIM_SetCompare1(TIMx, capture & 0xFFFF);
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);
}

void TIMER_SetCompare2(TIM_TypeDef* TIMx, unsigned long capture)
{
	capture += TIM_GetCounter(TIMx);
	TIM_SetCompare2(TIMx, capture & 0xFFFF);
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);
}

void TIMER_SetCompare3(TIM_TypeDef* TIMx, unsigned long capture)
{
	capture += TIM_GetCounter(TIMx);
	TIM_SetCompare3(TIMx, capture & 0xFFFF);
	TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);
}

void TIMER_SetCompare4(TIM_TypeDef* TIMx, unsigned long capture)
{
	capture += TIM_GetCounter(TIMx);
	TIM_SetCompare4(TIMx, capture & 0xFFFF);
	TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);
}

