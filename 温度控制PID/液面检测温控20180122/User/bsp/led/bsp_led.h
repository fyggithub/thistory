#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  LED_OFF = 0,
  LED_ON,
}LEDState_TypeDef;
#define IS_LED_STATE(STATE)           (((STATE) == LED_OFF) || ((STATE) == LED_ON))


/*
 * 以下宏定义内容跟开发板硬件息息相关，需要查看开发板电路原理图才能正确编写。
 * 例如，查原理图可在LED1灯接在stm32f103芯片的PB0引脚上，所以有关LED1的宏定义
 * 都是与GPIOB，GPIO_Pin_0相关的，我们专门把这些与开发板硬件相关的内容定义为宏，
 * 这对于修改或者移植程序非常方便。
 */

#define LED1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define LED1_GPIO_PIN                 GPIO_Pin_8
#define LED1_GPIO                     GPIOB

#define LED2_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define LED2_GPIO_PIN                 GPIO_Pin_11
#define LED2_GPIO                     GPIOB

#define NTC_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define NTC_GPIO_PIN                 GPIO_Pin_10
#define NTC_GPIO                     GPIOB

#define YM_OUTPUT_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define YM_OUTPUT_GPIO_PIN                 GPIO_Pin_6
#define YM_OUTPUT_GPIO                     GPIOB

#define YM_INPUT_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define YM_INPUT_GPIO_PIN                 GPIO_Pin_7
#define YM_INPUT_GPIO                     GPIOB

#define LED1_ON                       GPIO_ResetBits(LED1_GPIO,LED1_GPIO_PIN)
#define LED1_OFF                      GPIO_SetBits(LED1_GPIO,LED1_GPIO_PIN)
#define LED1_TOGGLE                   {LED1_GPIO->ODR ^=LED1_GPIO_PIN;}  //输出反转

#define LED2_ON                       GPIO_ResetBits(LED2_GPIO,LED2_GPIO_PIN)
#define LED2_OFF                      GPIO_SetBits(LED2_GPIO,LED2_GPIO_PIN)
#define LED2_TOGGLE                   {LED2_GPIO->ODR ^=LED2_GPIO_PIN;}  //输出反转

#define NTC_HEAT_PWM_ON                       GPIO_ResetBits(NTC_GPIO,NTC_GPIO_PIN)
#define NTC_HEAT_PWM_OFF                      GPIO_SetBits(NTC_GPIO,NTC_GPIO_PIN)
#define NTC_HEAT_PWM_TOGGLE                   {NTC_GPIO->ODR ^=NTC_GPIO_PIN;}  //输出反转

#define YM_OUTPUT_ON                       GPIO_SetBits(YM_OUTPUT_GPIO,YM_OUTPUT_GPIO_PIN)
#define YM_OUTPUT_OFF                      GPIO_ResetBits(YM_OUTPUT_GPIO,YM_OUTPUT_GPIO_PIN)
#define YM_OUTPUT_TOGGLE                   {YM_OUTPUT_GPIO->ODR ^=YM_OUTPUT_GPIO_PIN;}  //输出反转


/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void LED_GPIO_Init(void);
void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state);
#endif  // __BSP_LED_H__

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
