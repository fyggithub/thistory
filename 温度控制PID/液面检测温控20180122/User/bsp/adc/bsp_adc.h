#ifndef __ADC_H__
#define	__ADC_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include "bsp/DealPID/bsp_dealpid.h"

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd

#define    ADCx                          ADC1
#define    ADC_DMAx_CLK                  RCC_AHBPeriph_DMA1
#define    ADC_CLK                       RCC_APB2Periph_ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA
#define    ADC_PIN                       GPIO_Pin_4         
#define    ADC_CHANNEL                   ADC_Channel_4    

#define    ADJUST_ADC_PORT               GPIOA
#define    ADJUST_ADC_PIN                GPIO_Pin_5         
#define    ADJUST_ADC_CHANNEL            ADC_Channel_5

#define    DETECT_ADC_PORT               GPIOA
#define    DETECT_ADC_PIN                GPIO_Pin_6         
#define    DETECT_ADC_CHANNEL            ADC_Channel_6

void ADCx_Init(void);

#endif 

