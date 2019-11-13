/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: fyg
  * 版    本: V1.0
  * 编写日期: 2018/01/03
  * 功    能: 温度控制系统，利用PID算法，将PT1000采集到的数据进行处理，输出合理的PWM
			  使其 温度稳定在37摄氏度
			  
  ******************************************************************************
  */  
/* 包含头文件 ----------------------------------------------------------------*/
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

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{ 	
	HSE_SetSysClock(RCC_PLLMul_6 ); //设置主系统时钟
	MCO_GPIO_Config();
	RCC_MCOConfig(RCC_MCO_SYSCLK);
		
	BASIC_TIMER_Configuration();//定时器的初始化
	BASIC_PWM_Configuration();  //针预加热PWM控制端口
	pid_init();              	//PID数据初始化
	ADCx_Init();
	UsartSet_Init();         	//串口通信的初始化		
	USARTx_DMA_Config();        //DMA初始化
	MCP41010_Init();
	LED_GPIO_Init();
//	MCP41010_Write(0xFF);
	
	while (1)
	{       
		if(ADC_Stat.timeup_flag == 1)
		{
			ADC_Stat.timeup_flag = 0;
			Deal_Pid();
//			Display_AdcValue();   //输出日志
			
		}
    }
}



/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/
