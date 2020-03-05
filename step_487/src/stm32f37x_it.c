/**
  ******************************************************************************
  * @file    GPIO/GPIO_Toggle/stm32f37x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f37x_it.h"




void hook_CAN_isr(void);
void hook_CAN_err_isr(void);
void UART_isr(void);

/** @addtogroup STM32F37x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F37x Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	//CanRxMsg RxMessage = {0};
	//CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	hook_CAN_isr();
}
/**
* @brief  This function handles CAN1 RX1 request.
* @param  None
* @retval None
*/
void CAN1_RX1_IRQHandler(void)
{
	//CanRxMsg RxMessage = {0};
	//CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
}


/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	//UART_isr();
}

uint16_t capture = 0;
extern __IO uint16_t CCR1_Val;
extern __IO uint16_t CCR2_Val;
extern __IO uint16_t CCR3_Val;
extern __IO uint16_t CCR4_Val;


void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
		//capture = TIM_GetCapture1(TIM3);
		//TIM_SetCompare1(TIM3, capture + CCR1_Val );
		//step_isr(1);
		step_test();
	}

	/* TIM3_CH2 toggling with frequency = 1171.8 Hz */
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		//capture = TIM_GetCapture2(TIM3);
		//TIM_SetCompare2(TIM3, capture + CCR2_Val);
		step_isr(2);
	}

	/* TIM3_CH3 toggling with frequency = 2343.75 Hz */
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		//capture = TIM_GetCapture3(TIM3);
		//TIM_SetCompare3(TIM3, capture + CCR3_Val);
		//step_isr(2);
	}

	/* TIM3_CH4 toggling with frequency = 4687.5 Hz */
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		//capture = TIM_GetCapture4(TIM3);
		//TIM_SetCompare4(TIM3, capture + CCR4_Val);
		step_isr(6);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		//capture = TIM_GetCapture1(TIM2);
		//TIM_SetCompare1(TIM2, capture + CCR1_Val);
		//step_isr(4);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		//capture = TIM_GetCapture2(TIM2);
		//TIM_SetCompare2(TIM2, capture + CCR2_Val);
		step_isr(7);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		//capture = TIM_GetCapture3(TIM2);
		//TIM_SetCompare3(TIM2, capture + CCR2_Val);
		//step_isr(6);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		//capture = TIM_GetCapture4(TIM2);
		//TIM_SetCompare4(TIM2, capture + CCR2_Val);
		//step_isr(7);
	}
}

void TIM12_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM12, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM12, TIM_IT_CC2);
		//capture = TIM_GetCapture2(TIM12);
		//TIM_SetCompare2(TIM12, capture + CCR2_Val);
		step_isr(5);
	}
}

void TIM15_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM15, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM15, TIM_IT_CC2);
		//capture = TIM_GetCapture2(TIM15);
		//TIM_SetCompare2(TIM15, capture + CCR2_Val);
		
	}
}

void TIM19_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM19, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM19, TIM_IT_CC2);
		//capture = TIM_GetCapture2(TIM19);
		//TIM_SetCompare2(TIM19, capture + CCR2_Val);
		step_isr(4);
	}
}

void TIM13_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM13, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM13, TIM_IT_CC1);
		//capture = TIM_GetCapture1(TIM13);
		//TIM_SetCompare1(TIM13, capture + CCR1_Val);
		step_isr(3);
	}
}

void TIM14_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM13);
		TIM_SetCompare1(TIM13, capture + CCR1_Val);
		
		//step_isr(0);
		
	}
}

void TIM16_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM16, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM16, TIM_IT_CC1);
		//capture = TIM_GetCapture1(TIM16);
		//TIM_SetCompare1(TIM16, capture + CCR1_Val);
		
	}
}

void TIM17_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM17, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM17, TIM_IT_CC1);
		//capture = TIM_GetCapture1(TIM17);
		//TIM_SetCompare1(TIM17, capture + CCR1_Val);
	}
}

/******************************************************************************/
/*                 STM32F37x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f37x.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
