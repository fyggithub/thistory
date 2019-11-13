#include "LevelDetection.h"
#include "stateReady.h"

/********************************************************************
 *���ⲿ�ж�
 *******************************************************************/
VOID LiquidneedleEnable(VOID)
{
	MainState.Parameter.interruptFlag = 1;
//	HAL_NVIC_EnableIRQ(LIQUIDNEEDLE_EXTI_IRQ);	/* ʹ���ж� */	
}
/********************************************************************
 *�ر��ⲿ�ж�
 *******************************************************************/
VOID LiquidneedleDisable(VOID)
{
	MainState.Parameter.interruptFlag = 0;
//	HAL_NVIC_DisableIRQ(LIQUIDNEEDLE_EXTI_IRQ); /* �ر��ж� */	
}


/********************************************************************
 *�ⲿ�жϴ���ص�����
 *******************************************************************/
VOID HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(MainState.Parameter.interruptFlag == 1){
		Message msg;
		CloseLiquid();
		msg.id = MSG_DET_DONE;
		msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
	}		
}

/********************************************************************
 *�ⲿ�жϴ���
 *******************************************************************/
VOID LIQUIDNEEDLE_EXTI_IRQHandler(VOID)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);	
}








