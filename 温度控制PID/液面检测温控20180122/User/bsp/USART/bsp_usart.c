/**
  ******************************************************************************
  * 文件名程: bsp_usartx.c 
  * 作    者:
  * 版    本: V1.0
  * 编写日期: 
  * 功    能: 串口底层驱动程序
  ******************************************************************************
  **/

/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp/USART/bsp_usart.h"

unsigned char dailyrecord_buff[SENDBUFF_SIZE];	            

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 配置NVIC，设定USART接收中断优先级.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	 /* 嵌套向量中断控制器组选择 */  
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;	 /* 配置USART为中断源 */ 	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  /* 抢断优先级为0 */	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  /* 子优先级为1 */	 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* 使能中断 */	 
	NVIC_Init(&NVIC_InitStructure);   /* 初始化配置NVIC */
}

void DMA_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	 /* 嵌套向量中断控制器组选择 */  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;	 /* 配置USART为中断源 */ 	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* 抢断优先级为0 */	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  /* 子优先级为1 */	 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* 使能中断 */	 
	NVIC_Init(&NVIC_InitStructure);   /* 初始化配置NVIC */
}

/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用宏定义方法代替具体引脚号，方便程序移植，只要简单修改bsp_led.h
  *           文件相关宏定义就可以方便修改引脚。
  */
void UsartSet_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;   /* 定义IO硬件初始化结构体变量 */
	USART_InitTypeDef USART_InitStructure; /* 定义USART初始化结构体变量 */
 
	NVIC_Configuration();   /* 配置NVIC，设定USART接收中断优先级 */
	USARTx_ClockCmd(USARTx_CLK,ENABLE);  /* 使能USART时钟 */ 
	USARTx_GPIO_ClockCmd(USARTx_TX_CLK | USARTx_RX_CLK | RCC_APB2Periph_AFIO,ENABLE);   /* 使能USART功能GPIO时钟 */
  	
	GPIO_InitStructure.GPIO_Pin =  USARTx_TX_PIN;      /* 调试USART功能GPIO初始化 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    /* 设定USART发送对应IO模式：复用推挽输出 */ 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  /* 设定USART发送对应IO最大操作速度 ：GPIO_Speed_50MHz */ 
	GPIO_Init(USARTx_TX_PORT, &GPIO_InitStructure);    /* 初始化USART发送对应IO */ 

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;      /* 设定USART接收对应IO编号 */  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  /* 设定USART发送对应IO模式：浮空输入 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USARTx_RX_PORT, &GPIO_InitStructure);	   /* 初始化USART接收对应IO */
			
	/* USART工作环境配置 */ 
	USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;  /* USART波特率:115200 */ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  /* USART字长(有效位)：8位 */ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  /* USART停止位：1位 */
	USART_InitStructure.USART_Parity = USART_Parity_No ;    /* USART校验位：无 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   /* USART硬件数据流控制(硬件信号控制传输停止)：无 */	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   /* USART工作模式使能：允许接收和发送 */ 
	USART_Init(USARTx, &USART_InitStructure);  /* 初始化USART */

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);    /* 使能接收中断 */	
	USART_Cmd(USARTx, ENABLE);    /* 使能USART */
	USART_ClearFlag(USARTx, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);   /* 清除发送完成标志 */
}

void USARTx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  /* 开启DMA时钟 */
	DMA_NVIC_Configuration();
	
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USARTx->DR));  /*设置DMA源：串口数据寄存器地址*/ 	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)dailyrecord_buff;       /*内存地址(要传输的变量的指针)*/  		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	      /*方向：从内存到外设*/ 	
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;           /*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   /*外设地址不增*/  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	  /*内存地址自增*/  	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  /*外设数据单位*/  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	  /*内存数据单位 8bit*/ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	   /*DMA模式：不断循环*/ 	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;    /*优先级：中*/  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  /*禁止内存到内存的传输	*/		   
	DMA_Init(USARTx_DMA_CHANNEL, &DMA_InitStructure);  /*配置DMA1的4通道*/  
	DMA_Cmd (USARTx_DMA_CHANNEL,ENABLE);		  /*使能DMA*/	  
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);   /* 配置DMA发送完成后产生中断 */
}

/**
  * 函数功能: 串口发送一个字节数据 
  * 输入参数: ch：待发送字符
  * 返 回 值: 无
  * 说    明：无
  */
void Usart_SendByte(uint8_t ch)
{  
	USART_SendData(USARTx,ch);   /* 发送一个字节数据到USART1 */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	  /* 等待发送完毕 */
}

/**
  * 函数功能: 串口发送指定长度的字符串
  * 输入参数: str：待发送字符串缓冲器
  *           strlen:指定字符串长度
  * 返 回 值: 无
  * 说    明：无
  */
void Usart_SendStr_length(uint8_t *str,uint32_t strlen)
{
	unsigned int k = 0;
	do 
	{
		Usart_SendByte(*(str + k));
		k++;
	} while(k < strlen);
}

/**
  * 函数功能: 串口发送字符串，直到遇到字符串结束符
  * 输入参数: str：待发送字符串缓冲器
  * 返 回 值: 无
  * 说    明：无
  */
void Usart_SendString(uint8_t *str)
{
	unsigned int k = 0;
	do 
	{
		Usart_SendByte(*(str + k));
		k++;
	} while(*(str + k)!='\0');
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/



