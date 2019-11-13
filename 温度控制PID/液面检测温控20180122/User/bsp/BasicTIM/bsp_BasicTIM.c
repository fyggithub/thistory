/**
  ******************************************************************************
  * �ļ�����:
  * ��    ��: 
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ������ʱ��TIM6 & TIM7�ײ���������
  ******************************************************************************
  **/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/BasicTIM/bsp_BasicTIM.h" 

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: TIMx,x[6,7]�ж����ȼ����� 
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void BASIC_TIM3_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	/* �����ж���Ϊ0 */	 
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM3_IRQ;	 /* �����ж���Դ */ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  /* ���������ȼ�Ϊ 0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; /* ������ռ���ȼ�Ϊ2 */	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* �����ж�*/
	NVIC_Init(&NVIC_InitStructure);
}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         ����
 *	TIM_CounterMode			   TIMx,x[6,7]û�У���������
 *  TIM_Period             ����
 *  TIM_ClockDivision      TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter  TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
 
 //1msһ�ζ�ʱ���ж�
void BASIC_TIMER_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	BASIC_TIM3_APBxClock_FUN(BASIC_TIM3_CLK, ENABLE); /* ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M */
	BASIC_TIM3_NVIC_Configuration();  /* TIMx,x[6,7]�ж����ȼ����� */	
	TIM_TimeBaseStructure.TIM_Period = 1005;  /* �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ) */	
	TIM_TimeBaseStructure.TIM_Prescaler= 65;    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
													ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M */
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; /* ʱ�ӷ�Ƶ���� ��������ʱ��TIM6��TIM7û�У����ù� */	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  /* ����������ģʽ��������ʱ��TIM6��TIM7ֻ�����ϼ�����û�м���ģʽ�����ã����ù� */	
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  /* �ظ���������ֵ��������ʱ��TIM6��TIM7û�У����ù� */	
	TIM_TimeBaseInit(BASIC_TIM3, &TIM_TimeBaseStructure); /* ��ʼ����ʱ��TIMx, x[6,7] */	
	TIM_ClearITPendingBit(BASIC_TIM3, TIM_IT_Update);
	TIM_ClearFlag(BASIC_TIM3, TIM_FLAG_Update);  /* ����������жϱ�־λ */	
	TIM_ITConfig(BASIC_TIM3,TIM_IT_Update,ENABLE); /* �����������ж� */

	TIM_Cmd(BASIC_TIM3, ENABLE);  //ʹ�ܶ�ʱ��
}

void BASIC_TIM4_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	/* �����ж���Ϊ0 */	 
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM4_IRQ;	 /* �����ж���Դ */ 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  /* ���������ȼ�Ϊ 0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; /* ������ռ���ȼ�Ϊ2 */	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* �����ж�*/
	NVIC_Init(&NVIC_InitStructure);
}

void BASIC_PWM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	BASIC_TIM4_APBxClock_FUN(BASIC_TIM4_CLK, ENABLE); /* ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M */
	BASIC_TIM4_NVIC_Configuration();  /* TIMx,x[6,7]�ж����ȼ����� */	
	TIM_TimeBaseStructure.TIM_Period = 100;  /* �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ) */	
	TIM_TimeBaseStructure.TIM_Prescaler= 65;    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
													ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M */
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; /* ʱ�ӷ�Ƶ���� ��������ʱ��TIM6��TIM7û�У����ù� */	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* ����������ģʽ��������ʱ��TIM6��TIM7ֻ�����ϼ�����û�м���ģʽ�����ã����ù� */	
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  /* �ظ���������ֵ��������ʱ��TIM6��TIM7û�У����ù� */	
	TIM_TimeBaseInit(BASIC_TIM4, &TIM_TimeBaseStructure); /* ��ʼ����ʱ��TIMx, x[6,7] */	
	TIM_ClearITPendingBit(BASIC_TIM4, TIM_IT_Update);
	TIM_ClearFlag(BASIC_TIM4, TIM_FLAG_Update);  /* ����������жϱ�־λ */	
	TIM_ITConfig(BASIC_TIM4,TIM_IT_Update,ENABLE); /* �����������ж� */

	TIM_Cmd(BASIC_TIM4, ENABLE);  //ʹ�ܶ�ʱ��
}






/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/
