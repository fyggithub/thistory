/**
  ******************************************************************************
  * 文件名程: bsp_led.c 
  * 作    者: FYG
  * 版    本: V1.0
  * 编写日期: 20180119
  * 功    能: 板载LED灯底层驱动实现
  ******************************************************************************
 **
  */
/**
  * 很多初学者可能会疑惑bsp_led.c和bsp_led.h这两个文件哪里来的？？是不是官方下载
  * 的？？实际上，这两个文件是要求我们自己创建的，内容也是我们自己敲代码的。
  * 这个已经属于应用层次的代码ST官方是不提供的，需要我们根据自己的开发板硬件来
  * 编写我们的程序，所以需要我们自己新建文件编写。
  * 这里我们使用#include "bsp/led/bsp_led.h"而不是#include "bsp_led.h"是因为我们
  * 使用相对路径方法，可以省去在魔术棒(工程设置选项)的C/C++项目中添加文件的路径，
  * 使得移植程序更加方便。当然这样使用的前提是我们已经把"User"目录已经包含在工程
  * 中。
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp/led/bsp_led.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 板载LED灯IO引脚初始化.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用宏定义方法代替具体引脚号，方便程序移植，只要简单修改bsp_led.h
  *           文件相关宏定义就可以方便修改引脚。
  */
void LED_GPIO_Init(void)
{
	/* 定义IO硬件初始化结构体变量 */
	GPIO_InitTypeDef GPIO_InitStructure;
		 
	RCC_APB2PeriphClockCmd(LED1_RCC_CLOCKGPIO|LED2_RCC_CLOCKGPIO |NTC_RCC_CLOCKGPIO |YM_OUTPUT_RCC_CLOCKGPIO |YM_INPUT_RCC_CLOCKGPIO, ENABLE); /* 使能(开启)LED1引脚对应IO端口时钟 */ 

	
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;  	    /* 设定LED1对应引脚IO编号 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* 设定LED1对应引脚IO最大操作速度 ：GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* 设定LED1对应引脚IO为输出模式 */ 	
	GPIO_Init(LED1_GPIO, &GPIO_InitStructure);   /* 初始化LED1对应引脚IO */
	
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;    /* 设定LED2对应引脚IO编号 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* 设定LED1对应引脚IO最大操作速度 ：GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* 设定LED1对应引脚IO为输出模式 */
	GPIO_Init(LED2_GPIO, &GPIO_InitStructure);      /* 初始化LED2对应引脚IO */

	GPIO_InitStructure.GPIO_Pin = NTC_GPIO_PIN;    /* 设定NTC对应引脚IO编号 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* 设定LED1对应引脚IO最大操作速度 ：GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* 设定LED1对应引脚IO为输出模式 */
	GPIO_Init(NTC_GPIO, &GPIO_InitStructure);      /* 初始化NTC对应引脚IO */

	GPIO_InitStructure.GPIO_Pin = YM_OUTPUT_GPIO_PIN;    /* 设定NTC对应引脚IO编号 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* 设定LED1对应引脚IO最大操作速度 ：GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* 设定LED1对应引脚IO为输出模式 */
	GPIO_Init(YM_OUTPUT_GPIO, &GPIO_InitStructure);      /* 初始化NTC对应引脚IO */
	
	GPIO_InitStructure.GPIO_Pin = YM_INPUT_GPIO_PIN;    /* 设定NTC对应引脚IO编号 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   /* 设定LED1对应引脚IO最大操作速度 ：GPIO_Speed_50MHz */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    /* 设定LED1对应引脚IO为输出模式 */
	GPIO_Init(YM_INPUT_GPIO, &GPIO_InitStructure);      /* 初始化NTC对应引脚IO */
	
	LED1_OFF;
	LED2_OFF;
	NTC_HEAT_PWM_OFF;
	YM_OUTPUT_OFF;
}












