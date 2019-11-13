/**
  ******************************************************************************
  * 文件名程: bsp_GeneralTIM.c 
  * 作    者: 
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 通用定时器TIM3输出四路PWM底层驱动程序
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp/GeneralTIM/bsp_GeneralTIM.h" 

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 配置TIMx复用输出PWM时用到的I/O
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void GENERAL_TIMx_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GENERAL_TIM_APBxClock_FUN (GENERAL_TIM_CLK, ENABLE);  /* 使能定时器始终：设置TIM3CLK 为 72MHZ */
	GENERAL_TIM_GPIO_APBxClock_FUN(GENERAL_TIM_GPIO_CLK1|GENERAL_TIM_GPIO_CLK1, ENABLE);  /* 使能定时器通道引脚GPIO时钟 */
	
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  /* 配置定时器通道3输出引脚模式：复用推挽输出模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GENERAL_TIM_PORT1, &GPIO_InitStructure);	
}

/**
  * 函数功能: 配置TIMx输出的PWM信号的模式，如周期、极性、占空比
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 *    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 *
 */
static void GENERAL_TIMx_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 1005;   /* 当定时器从0计数到255，即为256次，为一个定时周期 */	
	TIM_TimeBaseStructure.TIM_Prescaler = 65; /* 设置预分频：不预分频，即为72MHz,输出脉冲频率：72MHz/(1999+1)/(255+1) */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;/* 设置时钟分频系数：不分频(这里用不到) */	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* 向上计数模式 */
	TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  /* 模式配置：PWM模式1 */	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /* 输出状态设置：使能输出 */		
	TIM_OCInitStructure.TIM_Pulse = 10;    /* 设置跳变值，当计数器计数到这个值时，电平发生跳变 */	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; /* 当定时器计数值小于CCR1_Val时为低电平 */ 	
	GENERAL_TIM_OCxInit(GENERAL_TIMx, &TIM_OCInitStructure);  /* 初始化定时器通道1输出PWM */	
	GENERAL_TIM_OCxPreloadConfig(GENERAL_TIMx, TIM_OCPreload_Enable);   /* 定时器比较输出通道1预装载配置：使能预装载 */	
	TIM_ARRPreloadConfig(GENERAL_TIMx, ENABLE);  /* 使能定时器重载寄存器ARR */	
	TIM_Cmd(GENERAL_TIMx, ENABLE);	/* 不使能定时器 */							
}

/**
  * 函数功能: TIMx输出PWM信号初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：只要调用这个函数TIMx的通道就会有PWM信号输出
  */
void GENERAL_TIMx_PWM_Init(void)
{
	GENERAL_TIMx_GPIO_Config();
	GENERAL_TIMx_Configuration();	
}


/******************* (C) COPYRIGHT *****END OF FILE****/
