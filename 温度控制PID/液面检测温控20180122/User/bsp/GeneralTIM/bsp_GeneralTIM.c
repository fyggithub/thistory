/**
  ******************************************************************************
  * �ļ�����: bsp_GeneralTIM.c 
  * ��    ��: 
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ͨ�ö�ʱ��TIM3�����·PWM�ײ���������
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/GeneralTIM/bsp_GeneralTIM.h" 

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����TIMx�������PWMʱ�õ���I/O
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void GENERAL_TIMx_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GENERAL_TIM_APBxClock_FUN (GENERAL_TIM_CLK, ENABLE);  /* ʹ�ܶ�ʱ��ʼ�գ�����TIM3CLK Ϊ 72MHZ */
	GENERAL_TIM_GPIO_APBxClock_FUN(GENERAL_TIM_GPIO_CLK1|GENERAL_TIM_GPIO_CLK1, ENABLE);  /* ʹ�ܶ�ʱ��ͨ������GPIOʱ�� */
	
	GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  /* ���ö�ʱ��ͨ��3�������ģʽ�������������ģʽ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GENERAL_TIM_PORT1, &GPIO_InitStructure);	
}

/**
  * ��������: ����TIMx�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 *    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 *
 */
static void GENERAL_TIMx_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 1005;   /* ����ʱ����0������255����Ϊ256�Σ�Ϊһ����ʱ���� */	
	TIM_TimeBaseStructure.TIM_Prescaler = 65; /* ����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz,�������Ƶ�ʣ�72MHz/(1999+1)/(255+1) */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;/* ����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���) */	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ���ϼ���ģʽ */
	TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  /* ģʽ���ã�PWMģʽ1 */	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /* ���״̬���ã�ʹ����� */		
	TIM_OCInitStructure.TIM_Pulse = 10;    /* ��������ֵ�������������������ֵʱ����ƽ�������� */	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; /* ����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ */ 	
	GENERAL_TIM_OCxInit(GENERAL_TIMx, &TIM_OCInitStructure);  /* ��ʼ����ʱ��ͨ��1���PWM */	
	GENERAL_TIM_OCxPreloadConfig(GENERAL_TIMx, TIM_OCPreload_Enable);   /* ��ʱ���Ƚ����ͨ��1Ԥװ�����ã�ʹ��Ԥװ�� */	
	TIM_ARRPreloadConfig(GENERAL_TIMx, ENABLE);  /* ʹ�ܶ�ʱ�����ؼĴ���ARR */	
	TIM_Cmd(GENERAL_TIMx, ENABLE);	/* ��ʹ�ܶ�ʱ�� */							
}

/**
  * ��������: TIMx���PWM�źų�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ֻҪ�����������TIMx��ͨ���ͻ���PWM�ź����
  */
void GENERAL_TIMx_PWM_Init(void)
{
	GENERAL_TIMx_GPIO_Config();
	GENERAL_TIMx_Configuration();	
}


/******************* (C) COPYRIGHT *****END OF FILE****/
