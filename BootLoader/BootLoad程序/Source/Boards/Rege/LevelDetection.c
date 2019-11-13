#include "LevelDetection.h"
#include "stateReady.h"

/********************************************************************
 *打开外部中断
 *******************************************************************/
VOID LiquidneedleEnable(VOID)
{
	MainState.Parameter.interruptFlag = 1;
//	HAL_NVIC_EnableIRQ(LIQUIDNEEDLE_EXTI_IRQ);	/* 使能中断 */	
}
/********************************************************************
 *关闭外部中断
 *******************************************************************/
VOID LiquidneedleDisable(VOID)
{
	MainState.Parameter.interruptFlag = 0;
//	HAL_NVIC_DisableIRQ(LIQUIDNEEDLE_EXTI_IRQ); /* 关闭中断 */	
}


/********************************************************************
 *外部中断处理回调函数
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
 *外部中断处理
 *******************************************************************/
VOID LIQUIDNEEDLE_EXTI_IRQHandler(VOID)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);	
}








