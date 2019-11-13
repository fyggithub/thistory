/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: fyg
  * ��    ��: V1.0
  * ��д����: 2018/01/03
  * ��    ��: �¶ȿ���ϵͳ������PID�㷨����PT1000�ɼ��������ݽ��д�����������PWM
			  ʹ�� �¶��ȶ���37���϶�
			  
  ******************************************************************************
  */  
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdlib.h"
#include "bsp/USART/bsp_usart.h"
#include "bsp/GeneralTIM/bsp_GeneralTIM.h"
#include "bsp/BasicTIM/bsp_BasicTIM.h" 
#include "bsp/PID/bsp_pid.h"
#include "bsp/adc/bsp_adc.h"
#include "bsp/MCP41010T/bsp_mcp41010.h"

#include "bsp/clkconfig/bsp_clkconfig.h"
#include "bsp/mcooutput/bsp_mcooutput.h"

#include "bsp/led/bsp_led.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{ 	
	HSE_SetSysClock(RCC_PLLMul_6 ); //������ϵͳʱ��
	MCO_GPIO_Config();
	RCC_MCOConfig(RCC_MCO_SYSCLK);
		
	BASIC_TIMER_Configuration();//��ʱ���ĳ�ʼ��
	BASIC_PWM_Configuration();  //��Ԥ����PWM���ƶ˿�
	pid_init();              	//PID���ݳ�ʼ��
	ADCx_Init();
	UsartSet_Init();         	//����ͨ�ŵĳ�ʼ��		
	USARTx_DMA_Config();        //DMA��ʼ��
	MCP41010_Init();
	LED_GPIO_Init();
//	MCP41010_Write(0xFF);
	
	while (1)
	{       
		if(ADC_Stat.timeup_flag == 1)
		{
			ADC_Stat.timeup_flag = 0;
			Deal_Pid();
//			Display_AdcValue();   //�����־
			
		}
    }
}



/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/
