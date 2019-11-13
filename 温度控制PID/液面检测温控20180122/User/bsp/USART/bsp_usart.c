/**
  ******************************************************************************
  * �ļ�����: bsp_usartx.c 
  * ��    ��:
  * ��    ��: V1.0
  * ��д����: 
  * ��    ��: ���ڵײ���������
  ******************************************************************************
  **/

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/USART/bsp_usart.h"

unsigned char dailyrecord_buff[SENDBUFF_SIZE];	            

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����NVIC���趨USART�����ж����ȼ�.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	 /* Ƕ�������жϿ�������ѡ�� */  
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;	 /* ����USARTΪ�ж�Դ */ 	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  /* �������ȼ�Ϊ0 */	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  /* �����ȼ�Ϊ1 */	 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* ʹ���ж� */	 
	NVIC_Init(&NVIC_InitStructure);   /* ��ʼ������NVIC */
}

void DMA_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	 /* Ƕ�������жϿ�������ѡ�� */  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;	 /* ����USARTΪ�ж�Դ */ 	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* �������ȼ�Ϊ0 */	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  /* �����ȼ�Ϊ1 */	 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* ʹ���ж� */	 
	NVIC_Init(&NVIC_InitStructure);   /* ��ʼ������NVIC */
}

/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_led.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void UsartSet_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;   /* ����IOӲ����ʼ���ṹ����� */
	USART_InitTypeDef USART_InitStructure; /* ����USART��ʼ���ṹ����� */
 
	NVIC_Configuration();   /* ����NVIC���趨USART�����ж����ȼ� */
	USARTx_ClockCmd(USARTx_CLK,ENABLE);  /* ʹ��USARTʱ�� */ 
	USARTx_GPIO_ClockCmd(USARTx_TX_CLK | USARTx_RX_CLK | RCC_APB2Periph_AFIO,ENABLE);   /* ʹ��USART����GPIOʱ�� */
  	
	GPIO_InitStructure.GPIO_Pin =  USARTx_TX_PIN;      /* ����USART����GPIO��ʼ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    /* �趨USART���Ͷ�ӦIOģʽ������������� */ 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  /* �趨USART���Ͷ�ӦIO�������ٶ� ��GPIO_Speed_50MHz */ 
	GPIO_Init(USARTx_TX_PORT, &GPIO_InitStructure);    /* ��ʼ��USART���Ͷ�ӦIO */ 

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;      /* �趨USART���ն�ӦIO��� */  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  /* �趨USART���Ͷ�ӦIOģʽ���������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USARTx_RX_PORT, &GPIO_InitStructure);	   /* ��ʼ��USART���ն�ӦIO */
			
	/* USART������������ */ 
	USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;  /* USART������:115200 */ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  /* USART�ֳ�(��Чλ)��8λ */ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  /* USARTֹͣλ��1λ */
	USART_InitStructure.USART_Parity = USART_Parity_No ;    /* USARTУ��λ���� */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   /* USARTӲ������������(Ӳ���źſ��ƴ���ֹͣ)���� */	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   /* USART����ģʽʹ�ܣ�������պͷ��� */ 
	USART_Init(USARTx, &USART_InitStructure);  /* ��ʼ��USART */

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);    /* ʹ�ܽ����ж� */	
	USART_Cmd(USARTx, ENABLE);    /* ʹ��USART */
	USART_ClearFlag(USARTx, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);   /* ���������ɱ�־ */
}

void USARTx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  /* ����DMAʱ�� */
	DMA_NVIC_Configuration();
	
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USARTx->DR));  /*����DMAԴ���������ݼĴ�����ַ*/ 	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)dailyrecord_buff;       /*�ڴ��ַ(Ҫ����ı�����ָ��)*/  		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	      /*���򣺴��ڴ浽����*/ 	
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;           /*�����СDMA_BufferSize=SENDBUFF_SIZE*/	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   /*�����ַ����*/  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	  /*�ڴ��ַ����*/  	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  /*�������ݵ�λ*/  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	  /*�ڴ����ݵ�λ 8bit*/ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	   /*DMAģʽ������ѭ��*/ 	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;    /*���ȼ�����*/  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  /*��ֹ�ڴ浽�ڴ�Ĵ���	*/		   
	DMA_Init(USARTx_DMA_CHANNEL, &DMA_InitStructure);  /*����DMA1��4ͨ��*/  
	DMA_Cmd (USARTx_DMA_CHANNEL,ENABLE);		  /*ʹ��DMA*/	  
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);   /* ����DMA������ɺ�����ж� */
}

/**
  * ��������: ���ڷ���һ���ֽ����� 
  * �������: ch���������ַ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Usart_SendByte(uint8_t ch)
{  
	USART_SendData(USARTx,ch);   /* ����һ���ֽ����ݵ�USART1 */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	  /* �ȴ�������� */
}

/**
  * ��������: ���ڷ���ָ�����ȵ��ַ���
  * �������: str���������ַ���������
  *           strlen:ָ���ַ�������
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: ���ڷ����ַ�����ֱ�������ַ���������
  * �������: str���������ַ���������
  * �� �� ֵ: ��
  * ˵    ������
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

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/



