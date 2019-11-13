#ifndef __BSP_BASICTIM_H__
#define __BSP_BASICTIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/

/********************基本定时器TIM参数定义***********/

#define BASIC_TIM3                 	TIM3
#define BASIC_TIM3_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define BASIC_TIM3_CLK              RCC_APB1Periph_TIM3
#define BASIC_TIM3_IRQ              TIM3_IRQn
#define BASIC_TIM3_INT_FUN          TIM3_IRQHandler

#define BASIC_TIM4                 	TIM4
#define BASIC_TIM4_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define BASIC_TIM4_CLK              RCC_APB1Periph_TIM4
#define BASIC_TIM4_IRQ              TIM4_IRQn
#define BASIC_TIM4_INT_FUN          TIM4_IRQHandler


void BASIC_TIM3_NVIC_Configuration(void);
void BASIC_TIMER_Configuration(void);
void BASIC_TIM4_NVIC_Configuration(void);
void BASIC_PWM_Configuration(void);


#endif	/* __BASIC_TIM_H__ */

