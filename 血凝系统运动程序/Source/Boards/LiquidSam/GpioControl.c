#include "Message.h"
#include "Liquid.h"
#include "Console.h"
#define LIQUIDNEEDLE_EXTI_IRQ				EXTI9_5_IRQn
#define LIQUIDNEEDLE_EXTI_IRQHandler        EXTI9_5_IRQHandler
STATIC UINT8 tag = 0;
STATIC UINT8 extiNum1 = 0;
STATIC UINT8 extiNum2 = 0;
extern UINT16 detOpen;
VOID HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	Message msg;
    if(0 == GpioGet(BoardGetGpio(F2LIQUID))){
        msg.id = MSG_DET_OPEN;
        msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
        MessagePost(&msg);
        extiNum2++;
        DebugLog(tag,"EXTI2:%d",extiNum2);
    }
    else{
        GpioSet(BoardGetGpio(L2FOURAXIS),1);
        detOpen = 0;
		extiNum1++;
		DebugLog(tag,"EXTI1:%d",extiNum1);
    }
    //HAL_NVIC_DisableIRQ(LIQUIDNEEDLE_EXTI_IRQ);
}
/********************************************************************
*
*******************************************************************/
VOID LIQUIDNEEDLE_EXTI_IRQHandler(VOID)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);	
}
/********************************************************************
 *end of file
 *******************************************************************/