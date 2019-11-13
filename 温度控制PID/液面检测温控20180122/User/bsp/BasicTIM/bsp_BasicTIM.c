/**
  ******************************************************************************
  * 文件名程:
  * 作    者: 
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 基本定时器TIM6 & TIM7底层驱动程序
  ******************************************************************************
  **/
/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp/BasicTIM/bsp_BasicTIM.h" 

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: TIMx,x[6,7]中断优先级配置 
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void BASIC_TIM3_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	/* 设置中断组为0 */	 
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM3_IRQ;	 /* 设置中断来源 */ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  /* 设置主优先级为 0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; /* 设置抢占优先级为2 */	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* 允许中断*/
	NVIC_Init(&NVIC_InitStructure);
}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         都有
 *	TIM_CounterMode			   TIMx,x[6,7]没有，其他都有
 *  TIM_Period             都有
 *  TIM_ClockDivision      TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter  TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
 
 //1ms一次定时器中断
void BASIC_TIMER_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	BASIC_TIM3_APBxClock_FUN(BASIC_TIM3_CLK, ENABLE); /* 开启TIMx_CLK,x[6,7],即内部时钟CK_INT=72M */
	BASIC_TIM3_NVIC_Configuration();  /* TIMx,x[6,7]中断优先级配置 */	
	TIM_TimeBaseStructure.TIM_Period = 1005;  /* 自动重装载寄存器周的值(计数值) */	
	TIM_TimeBaseStructure.TIM_Prescaler= 65;    /* 累计 TIM_Period个频率后产生一个更新或者中断
													时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M */
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; /* 时钟分频因子 ，基本定时器TIM6和TIM7没有，不用管 */	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  /* 计数器计数模式，基本定时器TIM6和TIM7只能向上计数，没有计数模式的设置，不用管 */	
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  /* 重复计数器的值，基本定时器TIM6和TIM7没有，不用管 */	
	TIM_TimeBaseInit(BASIC_TIM3, &TIM_TimeBaseStructure); /* 初始化定时器TIMx, x[6,7] */	
	TIM_ClearITPendingBit(BASIC_TIM3, TIM_IT_Update);
	TIM_ClearFlag(BASIC_TIM3, TIM_FLAG_Update);  /* 清除计数器中断标志位 */	
	TIM_ITConfig(BASIC_TIM3,TIM_IT_Update,ENABLE); /* 开启计数器中断 */

	TIM_Cmd(BASIC_TIM3, ENABLE);  //使能定时器
}

void BASIC_TIM4_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	/* 设置中断组为0 */	 
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM4_IRQ;	 /* 设置中断来源 */ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  /* 设置主优先级为 0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; /* 设置抢占优先级为2 */	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* 允许中断*/
	NVIC_Init(&NVIC_InitStructure);
}

void BASIC_PWM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	BASIC_TIM4_APBxClock_FUN(BASIC_TIM4_CLK, ENABLE); /* 开启TIMx_CLK,x[6,7],即内部时钟CK_INT=72M */
	BASIC_TIM4_NVIC_Configuration();  /* TIMx,x[6,7]中断优先级配置 */	
	TIM_TimeBaseStructure.TIM_Period = 100;  /* 自动重装载寄存器周的值(计数值) */	
	TIM_TimeBaseStructure.TIM_Prescaler= 65;    /* 累计 TIM_Period个频率后产生一个更新或者中断
													时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M */
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; /* 时钟分频因子 ，基本定时器TIM6和TIM7没有，不用管 */	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* 计数器计数模式，基本定时器TIM6和TIM7只能向上计数，没有计数模式的设置，不用管 */	
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  /* 重复计数器的值，基本定时器TIM6和TIM7没有，不用管 */	
	TIM_TimeBaseInit(BASIC_TIM4, &TIM_TimeBaseStructure); /* 初始化定时器TIMx, x[6,7] */	
	TIM_ClearITPendingBit(BASIC_TIM4, TIM_IT_Update);
	TIM_ClearFlag(BASIC_TIM4, TIM_FLAG_Update);  /* 清除计数器中断标志位 */	
	TIM_ITConfig(BASIC_TIM4,TIM_IT_Update,ENABLE); /* 开启计数器中断 */

	TIM_Cmd(BASIC_TIM4, ENABLE);  //使能定时器
}






/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/
