/*
 * arch_dac.c	(by xhl 2014/11/03)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f37x.h"



void DAC12_Config(void)
{
	DAC_InitTypeDef    DAC_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure PA.04 (DAC1_OUT1) in analog mode -------------------------*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable DAC clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC1, ENABLE);

	/* DAC1 channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits2_0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC1, DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC1, DAC_Channel_2, &DAC_InitStructure);

	/* Enable DAC1 Channel1 */
	DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC1, DAC_Channel_2, ENABLE);
	
}


void DAC_Config(void)
{
	DAC_InitTypeDef    DAC_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure PA.04 (DAC1_OUT1) in analog mode -------------------------*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable DAC clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC1, ENABLE);

	/* DAC1 channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits2_0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC1, DAC_Channel_1, &DAC_InitStructure);

	/* Enable DAC1 Channel1 */
	DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
}

#define DAC_VDDA	3300L
void DAC_SetVoltage(long Vout)
{
	long val;

	val = Vout * 4095L / DAC_VDDA;

	/* Output converted value on DAC1_OUT1 */
	DAC_SetChannel1Data(DAC1, DAC_Align_12b_R, val);
	DAC_SoftwareTriggerCmd(DAC1, DAC_Channel_1, ENABLE);
}

void DAC2_SetVoltage(long Vout)
{
	long val;

	val = Vout * 4095L / DAC_VDDA;

	/* Output converted value on DAC1_OUT1 */
	DAC_SetChannel2Data(DAC1, DAC_Align_12b_R, val);
	DAC_SoftwareTriggerCmd(DAC1, DAC_Channel_2, ENABLE);
}

