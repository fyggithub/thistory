#ifndef __BSP_MCP41010_H__
#define __BSP_MCP41010_H__

#include <stm32f10x.h>



#define MCP41010_RCC_CSGPIO            RCC_APB2Periph_GPIOB
#define MCP41010_CS_PIN                GPIO_Pin_12
#define MCP41010_CS_GPIO               GPIOB

#define MCP41010_RCC_CLKGPIO           RCC_APB2Periph_GPIOB
#define MCP41010_CLK_PIN               GPIO_Pin_13
#define MCP41010_CLK_GPIO              GPIOB

#define MCP41010_RCC_SIGPIO            RCC_APB2Periph_GPIOB
#define MCP41010_SI_PIN                GPIO_Pin_15
#define MCP41010_SI_GPIO               GPIOB



#define MCP41010_CS_ON                  GPIO_SetBits(MCP41010_CS_GPIO,MCP41010_CS_PIN)
#define MCP41010_CS_OFF                 GPIO_ResetBits(MCP41010_CS_GPIO,MCP41010_CS_PIN)
#define MCP41010_CS_TOGGLE              {MCP41010_CS_GPIO->ODR ^= MCP41010_CS_PIN;}     //输出反转

#define MCP41010_CLK_ON                  GPIO_SetBits(MCP41010_CLK_GPIO,MCP41010_CLK_PIN)
#define MCP41010_CLK_OFF                 GPIO_ResetBits(MCP41010_CLK_GPIO,MCP41010_CLK_PIN)
#define MCP41010_CLK_TOGGLE              {MCP41010_CLK_GPIO->ODR ^= MCP41010_CLK_PIN;}     //输出反转

#define MCP41010_SI_ON                  GPIO_SetBits(MCP41010_SI_GPIO,MCP41010_SI_PIN)
#define MCP41010_SI_OFF                 GPIO_ResetBits(MCP41010_SI_GPIO,MCP41010_SI_PIN)
#define MCP41010_SI_TOGGLE              {MCP41010_SI_GPIO->ODR ^= MCP41010_SI_PIN;}     //输出反转

void MCP41010_Init(void);
void MCP41010_Write(uint16_t write_data);
void delay_nop(uint16_t num_nop);
#endif



