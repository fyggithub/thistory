/**
  ******************************************************************************
  * 文件名程: bsp_mcooutput.c 
  * 作    者: 
  * 版    本: V1.0
  * 编写日期: 
  * 功    能: 配置MCO时钟输出引脚
  ******************************************************************************
  **/
/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp/mcooutput/bsp_mcooutput.h"
#include "stm32f10x_rcc.h"


/**
  * 函数功能: 初始化MCO引脚PA8
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：在F1系列中MCO引脚只有一个
  */
void MCO_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  // 开启GPIOA的时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  // 选择GPIO8引脚	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //设置为复用功能推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO的翻转速率为50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);  // 初始化GPIOA8
}
















/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/
