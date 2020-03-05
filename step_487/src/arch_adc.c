/*
 * arch_adc.c	(by xhl 2014/10/31)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "Alert.h"
#include "stm32f37x.h"

#define ADC1_DR_Address             0x4001244C

typedef struct {
	char *title;
	int	channel;
	int	sampletime;
	uint16_t	*ConvData;
}ADC_MAP;

ADC_MAP adcdev[] = {
	{ "Battery", ADC_Channel_Vbat, ADC_SampleTime_239Cycles5},
	{ "internal reference", ADC_Channel_Vrefint, ADC_SampleTime_55Cycles5},
	{ "Temperature sensor", ADC_Channel_TempSensor, ADC_SampleTime_55Cycles5},
	{ "DC24", ADC_Channel_2, ADC_SampleTime_55Cycles5},
	{ "DC12", ADC_Channel_3, ADC_SampleTime_55Cycles5},
};

#define COUNTOF(__s)	(sizeof(__s) / sizeof(__s[0]))

#define MAXCHANNEL	COUNTOF(adcdev)
uint16_t RegularConvData_Tab[MAXCHANNEL];

static void ADC_ChannelsConfig(void)
{
	int i;
	for(i = 0; i < MAXCHANNEL; i++) {
		ADC_RegularChannelConfig(ADC1, adcdev[i].channel, 1 + i, adcdev[i].sampletime);
		adcdev[i].ConvData = &RegularConvData_Tab[i];
	}
}

void ADC_config()
{
	ADC_InitTypeDef     ADC_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
	DMA_InitTypeDef     DMA_InitStructure;

	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure ADC Channel2 channel3 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* DMA1 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

	/* DMA1 Channel1 Config */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = MAXCHANNEL;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC1 DeInit */  
	ADC_DeInit(ADC1);

	/* Enable ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);  

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continuous mode */
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = MAXCHANNEL;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_ChannelsConfig();

	ADC_TempSensorVrefintCmd(ENABLE);

	SYSCFG_VBATMonitoringCmd(ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);  

	/* ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);

	while(ADC_GetResetCalibrationStatus(ADC1));

	/* ADC1 calibration start */
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1)); 

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); 
}

void arch_adc_test()
{
	int i;
	uint32_t v, mv;
	extern volatile unsigned int DC24;
	extern volatile unsigned int DC12;
	extern int alert_push(int alert_code, int alert_arg);
	unsigned int dc_error =0;

	/* Test DMA1 TC flag */
	if((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET) return;

	/* Clear DMA TC flag */
	DMA_ClearFlag(DMA1_FLAG_TC1);

	for(i = 0; i < MAXCHANNEL; i++)
	{
		mv = *adcdev[i].ConvData;
		if (i == 0) {
			mv *= 2;
		}
		v = (mv * 3300L) / 0xFFF;

		if (i==3)
		{
			DC24 = v;

			if (DC24<DC24_MIN){
				dc_error = YARN_DC24_LOW;
			}
			if (DC24>DC24_MAX){
				dc_error = YARN_DC24_HIGH;
			}

			if (dc_error!=0)
			{
				alert_push(dc_error,0);
				dc_error =0;
			}
		}
		if (i==4)
		{
			DC12 = v;
			if (DC12<DC12_MIN){
				dc_error = YARN_DC12_LOW;
			}
			if (DC12>DC12_MAX){
				dc_error = YARN_DC12_HIGH;
			}
			if (dc_error!=0)
			{
				alert_push(dc_error,0);
				dc_error =0;
			}
			
		}

		//printf("ADC(%s) = %0.3f V  \n\r",adcdev[i].title, (float)v / 1000);
	} 
}

