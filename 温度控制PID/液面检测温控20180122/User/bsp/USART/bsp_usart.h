#ifndef __BSP_USART_H__
#define __BSP_USART_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

/* 类型定义 ------------------------------------------------------------------*/


/* 宏定义 --------------------------------------------------------------------*/
#define SENDBUFF_SIZE                        59
 
#define USARTx_BAUDRATE                      115200

#define USARTx_ClockCmd                      RCC_APB1PeriphClockCmd
#define USARTx_CLK                           RCC_APB1Periph_USART2

#define USARTx_GPIO_ClockCmd                 RCC_APB2PeriphClockCmd
    
#define USARTx_TX_PORT                       GPIOA   
#define USARTx_TX_PIN                        GPIO_Pin_2
#define USARTx_TX_CLK                        RCC_APB2Periph_GPIOA 

#define USARTx_RX_PORT                       GPIOA 
#define USARTx_RX_PIN                        GPIO_Pin_3
#define USARTx_RX_CLK                        RCC_APB2Periph_GPIOA

#define USARTx_IRQHANDLER                    USART2_IRQHandler
#define USARTx_IRQn                          USART2_IRQn
#define USARTx                               USART2
#define USARTx_DMA_CHANNEL                   DMA1_Channel4


/* 扩展变量 ------------------------------------------------------------------*/





/* 函数声明 ------------------------------------------------------------------*/
void UsartSet_Init(void);
void Usart_SendByte(uint8_t ch);
void Usart_SendStr_length(uint8_t *str,uint32_t strlen);
void Usart_SendString(uint8_t *str);

void DMA_NVIC_Configuration(void);
void USARTx_DMA_Config(void);
//void Usart_SendByte(uint16_t ch);
//void Usart_SendStr_length(uint16_t *str,uint16_t strlen);
//void Usart_SendString(uint8_t *str);

extern unsigned char dailyrecord_buff[SENDBUFF_SIZE];

#endif 

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/





