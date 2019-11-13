/**
  ******************************************************************************
  * �ļ�����: bsp_adc.c 
  * ��    ��: fyg
  * ��    ��: V1.0
  * ��д����: 2018-1-18
  * ��    ��: ����ADC��ѹ�ɼ��ײ���������
  ******************************************************************************
  */
#include "bsp/adc/bsp_adc.h"




/**
  * ��������: ADC GPIO ��ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE );  /* �� ADC IO�˿�ʱ�� */
		
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;          /* ���� ADC IO ����ģʽ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);       /* ��ʼ�� ADC IO */		 		

	GPIO_InitStructure.GPIO_Pin = ADJUST_ADC_PIN;          /* ���� ADC IO ����ģʽ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(ADJUST_ADC_PORT, &GPIO_InitStructure);       /* ��ʼ�� ADC IO */	

	GPIO_InitStructure.GPIO_Pin = DETECT_ADC_PIN;          /* ���� ADC IO ����ģʽ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(DETECT_ADC_PORT, &GPIO_InitStructure);       /* ��ʼ�� ADC IO */
}

/**
  * ��������: NVIC���ã�ADC�ж����ȼ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
//static void ADC_NVIC_Config(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//    
//  NVIC_InitStructure.NVIC_IRQChannel = ADCx_IRQ;  /* �����ж����ȼ� */ 	 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* ������ռʽ���ȼ�Ϊ0 */	 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   /* ���������ȼ�Ϊ0 */	 
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* ʹ���ⲿ�ж�ͨ�� */	 
//  NVIC_Init(&NVIC_InitStructure);   /* ��ʼ������Ƕ�������жϿ����� */
//}

/**
  * ��������: ����ADC����ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
//static void ADCx_Mode_Config(void)
//{
//	ADC_InitTypeDef ADC_InitStructure;	
//	
//	ADC_APBxClock_FUN(ADC_CLK,ENABLE );	  /* ��ADCʱ�� */
//	/* ADC ģʽ���� */
//	
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	/* ֻʹ��һ��ADC�����ڵ�ģʽ */	
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE ;   /* ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ */	
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  /* ����ת��ģʽ */	
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /* �����ⲿ����ת��������������� */	
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 /* ת������Ҷ��� */	
//	ADC_InitStructure.ADC_NbrOfChannel = 1;		/* ת��ͨ��1�� */	
//	ADC_Init(ADCx, &ADC_InitStructure);   /* ��ʼ��ADC */
//		
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   /* ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz */
//	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);  /* ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ55.5��ʱ������ */	
//	ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);  /* ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ */
//	ADC_Cmd(ADCx, ENABLE);  /* ����ADC ������ʼת�� */	
//	ADC_ResetCalibration(ADCx);  /* ��ʼ��ADC У׼�Ĵ���   */	
//	while(ADC_GetResetCalibrationStatus(ADCx));	  /*�ȴ�У׼�Ĵ�����ʼ����� */	
//	ADC_StartCalibration(ADCx);  /* ADC��ʼУ׼*/	
//	while(ADC_GetCalibrationStatus(ADCx));  /*�ȴ�У׼��� */

//	ADC_SoftwareStartConvCmd(ADCx, ENABLE);  /* ����û�в����ⲿ����������ʹ���������ADCת��  */
//}

/**
  * ��������:  ����ADC����ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	DMA_InitTypeDef DMA_InitStructure;

	ADC_APBxClock_FUN(ADC_CLK,ENABLE );   /* ��ADCʱ�� */	
	RCC_AHBPeriphClockCmd(ADC_DMAx_CLK, ENABLE);  /* ��DMAʱ�� */
	DMA_DeInit(ADC_DMA_CHANNEL);    /* ��λDMA������ */
	
	/* ���� DMA ��ʼ���ṹ�� */	
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(ADCx->DR));	  /* �����ַΪ��ADC ���ݼĴ�����ַ */	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Parameter.channelvalue;	 /* �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı��� */	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	 /* ����Դ�������� */	
	DMA_InitStructure.DMA_BufferSize = 3;
//	DMA_InitStructure.DMA_BufferSize = 1;	  /* ���ݳ��� */	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* ����Ĵ���ֻ��һ������ַ���õ��� */	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 	 /* �洢����ַ�̶� */	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	 /* �������ݴ�СΪ���֣��������ֽ� */	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	 /* �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ */	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	   /* ѭ������ģʽ */	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	  /* DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ�� */	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	 /* ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢�� */
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);   /* ��ʼ��DMA */	
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);     /* ʹ�� DMA ͨ�� */
  
	/* ADC ģʽ���� */	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 /* ֻʹ��һ��ADC�����ڵ�ģʽ */	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE ;       /* ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ */	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   /* ����ת��ģʽ */	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   /* �����ⲿ����ת��������������� */	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	  /* ת������Ҷ��� */	
	ADC_InitStructure.ADC_NbrOfChannel = 3;
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	   /* ת��ͨ��1�� */			
	ADC_Init(ADCx, &ADC_InitStructure);        /* ��ʼ��ADC */
		
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);      /* ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz */
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);   /* ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ55.5��ʱ������ */ 
	ADC_RegularChannelConfig(ADCx, ADJUST_ADC_CHANNEL, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx, DETECT_ADC_CHANNEL, 3, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADCx, ENABLE);     /* ʹ��ADC DMA ���� */
	ADC_Cmd(ADCx, ENABLE);        /* ����ADC ������ʼת�� */
	
	ADC_ResetCalibration(ADCx);    /* ��ʼ��ADC У׼�Ĵ���   */	
	while(ADC_GetResetCalibrationStatus(ADCx));	  /*�ȴ�У׼�Ĵ�����ʼ����� */	
	ADC_StartCalibration(ADCx);    /* ADC��ʼУ׼*/
	while(ADC_GetCalibrationStatus(ADCx));  /*�ȴ�У׼��� */

	ADC_SoftwareStartConvCmd(ADCx, ENABLE);  /* ����û�в����ⲿ����������ʹ���������ADCת��  */
}

/**
  * ��������: ADC��ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();	
}


