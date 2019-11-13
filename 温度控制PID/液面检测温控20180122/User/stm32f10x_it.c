/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp/BasicTIM/bsp_BasicTIM.h" 
#include "bsp/GeneralTIM/bsp_GeneralTIM.h"
#include "bsp/USART/bsp_usart.h"
#include "bsp/DealPID/bsp_dealpid.h"
#include "bsp/led/bsp_led.h"

static uint32_t gCurrSysTick = 0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
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
	gCurrSysTick ++;
}

uint32_t GetSysTick(void)
{
	return gCurrSysTick;
}

void USARTx_IRQHANDLER(void)
{	
	static uint16_t data_count = 0;
			
	if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
	{ 	
		PID_Parameter.Sum_Buff[data_count++] = USART_ReceiveData(USARTx);
		
		if(data_count >= 3)
		{
			data_count = 0;
			PID_Parameter.P_SumValue = PID_Parameter.Sum_Buff[0];
			PID_Parameter.I_SumValue = PID_Parameter.Sum_Buff[1];
			PID_Parameter.D_SumValue = PID_Parameter.Sum_Buff[2];
		}		
	} 
}

void DMA1_Channel4_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC4) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);		
		DMA_Cmd (USARTx_DMA_CHANNEL,DISABLE);	
		DMA_ClearFlag(DMA1_IT_TC4);
	}
}

//1ms进一次中断
void  BASIC_TIM3_INT_FUN (void)
{
	static unsigned int pid_timercount = 0,readad_timecount = 0,change_hzcount = 0,hz_num = 0;

	if ( TIM_GetITStatus( BASIC_TIM3, TIM_IT_Update) != RESET ) 
	{	
		pid_timercount++;
		readad_timecount++;
		change_hzcount++;
		
		if(readad_timecount >= 10)  //10ms读一次值
		{
			readad_timecount = 0;
			Read_AdcValue();       //将采集的数据按照需求进行存储处理
		}
		
		if(pid_timercount >= 100)     //100ms处理一次PID
		{
			pid_timercount = 0;				
			LED1_TOGGLE;
			ADC_Stat.timeup_flag = 1;						
		}
		if(change_hzcount >= 1000)
		{
			change_hzcount = 0;
			MCP41010_Write(hz_num++);
			if(hz_num > 255)
				hz_num = 0;
		}
		
		TIM_ClearITPendingBit(BASIC_TIM3 , TIM_IT_Update);  		 
	}		 	
}




//发出PWM,频率大约为1KHz,
void  BASIC_TIM4_INT_FUN (void)
{
	static unsigned int readad_timecount = 0;
	if ( TIM_GetITStatus( BASIC_TIM4, TIM_IT_Update) != RESET ) 
	{
		readad_timecount++;
		if(readad_timecount == PWM_Pameter.count_value)
		{			
			NTC_HEAT_PWM_OFF;			
		}
		else if(readad_timecount >= 100)     
		{
			if(PWM_Pameter.count_value == 0)
				NTC_HEAT_PWM_OFF;
			else
				NTC_HEAT_PWM_ON;
			readad_timecount = 0;								
		}
		TIM_ClearITPendingBit(BASIC_TIM4 , TIM_IT_Update);
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
