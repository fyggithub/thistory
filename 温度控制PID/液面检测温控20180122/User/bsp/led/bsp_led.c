/**
  ******************************************************************************
  * �ļ�����: bsp_led.c 
  * ��    ��: FYG
  * ��    ��: V1.0
  * ��д����: 20180119
  * ��    ��: ����LED�Ƶײ�����ʵ��
  ******************************************************************************
 **
  */
/**
  * �ܶ��ѧ�߿��ܻ��ɻ�bsp_led.c��bsp_led.h�������ļ��������ģ����ǲ��ǹٷ�����
  * �ģ���ʵ���ϣ��������ļ���Ҫ�������Լ������ģ�����Ҳ�������Լ��ô���ġ�
  * ����Ѿ�����Ӧ�ò�εĴ���ST�ٷ��ǲ��ṩ�ģ���Ҫ���Ǹ����Լ��Ŀ�����Ӳ����
  * ��д���ǵĳ���������Ҫ�����Լ��½��ļ���д��
  * ��������ʹ��#include "bsp/led/bsp_led.h"������#include "bsp_led.h"����Ϊ����
  * ʹ�����·������������ʡȥ��ħ����(��������ѡ��)��C/C++��Ŀ������ļ���·����
  * ʹ����ֲ������ӷ��㡣��Ȼ����ʹ�õ�ǰ���������Ѿ���"User"Ŀ¼�Ѿ������ڹ���
  * �С�
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/led/bsp_led.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����LED��IO���ų�ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_led.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void LED_GPIO_Init(void)
{
	/* ����IOӲ����ʼ���ṹ����� */
	GPIO_InitTypeDef GPIO_InitStructure;
		 
	RCC_APB2PeriphClockCmd(LED1_RCC_CLOCKGPIO|LED2_RCC_CLOCKGPIO |NTC_RCC_CLOCKGPIO |YM_OUTPUT_RCC_CLOCKGPIO |YM_INPUT_RCC_CLOCKGPIO, ENABLE); /* ʹ��(����)LED1���Ŷ�ӦIO�˿�ʱ�� */ 

	
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;  	    /* �趨LED1��Ӧ����IO��� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* �趨LED1��Ӧ����IOΪ���ģʽ */ 	
	GPIO_Init(LED1_GPIO, &GPIO_InitStructure);   /* ��ʼ��LED1��Ӧ����IO */
	
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;    /* �趨LED2��Ӧ����IO��� */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* �趨LED1��Ӧ����IOΪ���ģʽ */
	GPIO_Init(LED2_GPIO, &GPIO_InitStructure);      /* ��ʼ��LED2��Ӧ����IO */

	GPIO_InitStructure.GPIO_Pin = NTC_GPIO_PIN;    /* �趨NTC��Ӧ����IO��� */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* �趨LED1��Ӧ����IOΪ���ģʽ */
	GPIO_Init(NTC_GPIO, &GPIO_InitStructure);      /* ��ʼ��NTC��Ӧ����IO */

	GPIO_InitStructure.GPIO_Pin = YM_OUTPUT_GPIO_PIN;    /* �趨NTC��Ӧ����IO��� */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* �趨LED1��Ӧ����IOΪ���ģʽ */
	GPIO_Init(YM_OUTPUT_GPIO, &GPIO_InitStructure);      /* ��ʼ��NTC��Ӧ����IO */
	
	GPIO_InitStructure.GPIO_Pin = YM_INPUT_GPIO_PIN;    /* �趨NTC��Ӧ����IO��� */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* �趨LED1��Ӧ����IO�������ٶ� ��GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    /* �趨LED1��Ӧ����IOΪ���ģʽ */
	GPIO_Init(YM_INPUT_GPIO, &GPIO_InitStructure);      /* ��ʼ��NTC��Ӧ����IO */
	
	LED1_OFF;
	LED2_OFF;
	NTC_HEAT_PWM_OFF;
	YM_OUTPUT_OFF;
}












