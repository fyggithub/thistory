#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum
{
  LED_OFF = 0,
  LED_ON,
}LEDState_TypeDef;
#define IS_LED_STATE(STATE)           (((STATE) == LED_OFF) || ((STATE) == LED_ON))


/*
 * ���º궨�����ݸ�������Ӳ��ϢϢ��أ���Ҫ�鿴�������·ԭ��ͼ������ȷ��д��
 * ���磬��ԭ��ͼ����LED1�ƽ���stm32f103оƬ��PB0�����ϣ������й�LED1�ĺ궨��
 * ������GPIOB��GPIO_Pin_0��صģ�����ר�Ű���Щ�뿪����Ӳ����ص����ݶ���Ϊ�꣬
 * ������޸Ļ�����ֲ����ǳ����㡣
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
#define LED1_TOGGLE                   {LED1_GPIO->ODR ^=LED1_GPIO_PIN;}  //�����ת

#define LED2_ON                       GPIO_ResetBits(LED2_GPIO,LED2_GPIO_PIN)
#define LED2_OFF                      GPIO_SetBits(LED2_GPIO,LED2_GPIO_PIN)
#define LED2_TOGGLE                   {LED2_GPIO->ODR ^=LED2_GPIO_PIN;}  //�����ת

#define NTC_HEAT_PWM_ON                       GPIO_ResetBits(NTC_GPIO,NTC_GPIO_PIN)
#define NTC_HEAT_PWM_OFF                      GPIO_SetBits(NTC_GPIO,NTC_GPIO_PIN)
#define NTC_HEAT_PWM_TOGGLE                   {NTC_GPIO->ODR ^=NTC_GPIO_PIN;}  //�����ת

#define YM_OUTPUT_ON                       GPIO_SetBits(YM_OUTPUT_GPIO,YM_OUTPUT_GPIO_PIN)
#define YM_OUTPUT_OFF                      GPIO_ResetBits(YM_OUTPUT_GPIO,YM_OUTPUT_GPIO_PIN)
#define YM_OUTPUT_TOGGLE                   {YM_OUTPUT_GPIO->ODR ^=YM_OUTPUT_GPIO_PIN;}  //�����ת


/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void LED_GPIO_Init(void);
void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state);
#endif  // __BSP_LED_H__

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
