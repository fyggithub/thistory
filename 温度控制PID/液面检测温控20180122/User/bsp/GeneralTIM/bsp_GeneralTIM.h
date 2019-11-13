#ifndef __GENERAL_TIM_H__
#define __GENERAL_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/************* ͨ�ö�ʱ��TIM�������壬ֻ��TIM2 & TIM3 & TIM4 & TIM5 ***********/
#define GENERAL_TIMx                        TIM4
#define GENERAL_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK                     RCC_APB1Periph_TIM4
#define GENERAL_TIM_GPIO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define GENERAL_TIM_GPIO_CLK1               RCC_APB2Periph_GPIOB
#define GENERAL_TIM_PORT1                   GPIOB
#define GENERAL_TIM_PIN1                    GPIO_Pin_9
#define GENERAL_TIM_OCxInit                 TIM_OC4Init
#define GENERAL_TIM_OCxPreloadConfig        TIM_OC4PreloadConfig
#define GENERAL_TIM_CCRx                    CCR4
#define GENERAL_TIMx_IRQn                   TIM4_IRQn              //�ж�
#define GENERAL_TIMx_IRQHandler             TIM4_IRQHandler

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void GENERAL_TIMx_PWM_Init(void);

#endif	/* __GENERAL_TIM_H__ */
