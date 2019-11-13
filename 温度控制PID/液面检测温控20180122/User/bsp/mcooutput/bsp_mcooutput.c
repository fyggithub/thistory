/**
  ******************************************************************************
  * �ļ�����: bsp_mcooutput.c 
  * ��    ��: 
  * ��    ��: V1.0
  * ��д����: 
  * ��    ��: ����MCOʱ���������
  ******************************************************************************
  **/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/mcooutput/bsp_mcooutput.h"
#include "stm32f10x_rcc.h"


/**
  * ��������: ��ʼ��MCO����PA8
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������F1ϵ����MCO����ֻ��һ��
  */
void MCO_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  // ����GPIOA��ʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  // ѡ��GPIO8����	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //����Ϊ���ù����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ķ�ת����Ϊ50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);  // ��ʼ��GPIOA8
}
















/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/
