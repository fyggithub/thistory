/**
  ******************************************************************************
  * 文件名程: bsp_adc.c 
  * 作    者: fyg
  * 版    本: V1.0
  * 编写日期: 2018-1-18
  * 功    能: 板载ADC电压采集底层驱动程序
  ******************************************************************************
  */
#include "bsp/adc/bsp_adc.h"




/**
  * 函数功能: ADC GPIO 初始化.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE );  /* 打开 ADC IO端口时钟 */
		
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;          /* 配置 ADC IO 引脚模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);       /* 初始化 ADC IO */		 		

	GPIO_InitStructure.GPIO_Pin = ADJUST_ADC_PIN;          /* 配置 ADC IO 引脚模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(ADJUST_ADC_PORT, &GPIO_InitStructure);       /* 初始化 ADC IO */	

	GPIO_InitStructure.GPIO_Pin = DETECT_ADC_PIN;          /* 配置 ADC IO 引脚模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(DETECT_ADC_PORT, &GPIO_InitStructure);       /* 初始化 ADC IO */
}

/**
  * 函数功能: NVIC配置：ADC中断优先级配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
//static void ADC_NVIC_Config(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//    
//  NVIC_InitStructure.NVIC_IRQChannel = ADCx_IRQ;  /* 配置中断优先级 */ 	 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* 设置抢占式优先级为0 */	 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   /* 设置子优先级为0 */	 
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* 使能外部中断通道 */	 
//  NVIC_Init(&NVIC_InitStructure);   /* 初始化配置嵌套向量中断控制器 */
//}

/**
  * 函数功能: 配置ADC工作模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
//static void ADCx_Mode_Config(void)
//{
//	ADC_InitTypeDef ADC_InitStructure;	
//	
//	ADC_APBxClock_FUN(ADC_CLK,ENABLE );	  /* 打开ADC时钟 */
//	/* ADC 模式配置 */
//	
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	/* 只使用一个ADC，属于单模式 */	
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE ;   /* 禁止扫描模式，多通道才要，单通道不需要 */	
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  /* 连续转换模式 */	
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /* 不用外部触发转换，软件开启即可 */	
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 /* 转换结果右对齐 */	
//	ADC_InitStructure.ADC_NbrOfChannel = 1;		/* 转换通道1个 */	
//	ADC_Init(ADCx, &ADC_InitStructure);   /* 初始化ADC */
//		
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   /* 配置ADC时钟为PCLK2的8分频，即9MHz */
//	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);  /* 配置 ADC 通道转换顺序为1，第一个转换，采样时间为55.5个时钟周期 */	
//	ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);  /* ADC 转换结束产生中断，在中断服务程序中读取转换值 */
//	ADC_Cmd(ADCx, ENABLE);  /* 开启ADC ，并开始转换 */	
//	ADC_ResetCalibration(ADCx);  /* 初始化ADC 校准寄存器   */	
//	while(ADC_GetResetCalibrationStatus(ADCx));	  /*等待校准寄存器初始化完成 */	
//	ADC_StartCalibration(ADCx);  /* ADC开始校准*/	
//	while(ADC_GetCalibrationStatus(ADCx));  /*等待校准完成 */

//	ADC_SoftwareStartConvCmd(ADCx, ENABLE);  /* 由于没有采用外部触发，所以使用软件触发ADC转换  */
//}

/**
  * 函数功能:  配置ADC工作模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	DMA_InitTypeDef DMA_InitStructure;

	ADC_APBxClock_FUN(ADC_CLK,ENABLE );   /* 打开ADC时钟 */	
	RCC_AHBPeriphClockCmd(ADC_DMAx_CLK, ENABLE);  /* 打开DMA时钟 */
	DMA_DeInit(ADC_DMA_CHANNEL);    /* 复位DMA控制器 */
	
	/* 配置 DMA 初始化结构体 */	
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(ADCx->DR));	  /* 外设基址为：ADC 数据寄存器地址 */	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Parameter.channelvalue;	 /* 存储器地址，实际上就是一个内部SRAM的变量 */	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	 /* 数据源来自外设 */	
	DMA_InitStructure.DMA_BufferSize = 3;
//	DMA_InitStructure.DMA_BufferSize = 1;	  /* 数据长度 */	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* 外设寄存器只有一个，地址不用递增 */	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 	 /* 存储器地址固定 */	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	 /* 外设数据大小为半字，即两个字节 */	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	 /* 内存数据大小也为半字，跟外设数据大小相同 */	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	   /* 循环传输模式 */	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	  /* DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响 */	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	 /* 禁止存储器到存储器模式，因为是从外设到存储器 */
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);   /* 初始化DMA */	
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);     /* 使能 DMA 通道 */
  
	/* ADC 模式配置 */	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 /* 只使用一个ADC，属于单模式 */	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE ;       /* 禁止扫描模式，多通道才要，单通道不需要 */	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   /* 连续转换模式 */	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   /* 不用外部触发转换，软件开启即可 */	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	  /* 转换结果右对齐 */	
	ADC_InitStructure.ADC_NbrOfChannel = 3;
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	   /* 转换通道1个 */			
	ADC_Init(ADCx, &ADC_InitStructure);        /* 初始化ADC */
		
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);      /* 配置ADC时钟为PCLK2的8分频，即9MHz */
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);   /* 配置 ADC 通道转换顺序为1，第一个转换，采样时间为55.5个时钟周期 */ 
	ADC_RegularChannelConfig(ADCx, ADJUST_ADC_CHANNEL, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx, DETECT_ADC_CHANNEL, 3, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADCx, ENABLE);     /* 使能ADC DMA 请求 */
	ADC_Cmd(ADCx, ENABLE);        /* 开启ADC ，并开始转换 */
	
	ADC_ResetCalibration(ADCx);    /* 初始化ADC 校准寄存器   */	
	while(ADC_GetResetCalibrationStatus(ADCx));	  /*等待校准寄存器初始化完成 */	
	ADC_StartCalibration(ADCx);    /* ADC开始校准*/
	while(ADC_GetCalibrationStatus(ADCx));  /*等待校准完成 */

	ADC_SoftwareStartConvCmd(ADCx, ENABLE);  /* 由于没有采用外部触发，所以使用软件触发ADC转换  */
}

/**
  * 函数功能: ADC初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();	
}


