/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Pulse.h
 *******************************************************************/
#ifndef _PULSE_H_
#define _PULSE_H_
/********************************************************************
 * Pulse struct
 *******************************************************************/
#include "Datatypes.h"
#include "stm32f1xx_hal.h"

enum
{
	PULSE01 =0, PULSE02, PULSE03, PULSE04, PULSEADD,PULSE_MAX,
};
struct __Pulse;
typedef VOID (*PULSE_CB)(struct __Pulse* pulse);
typedef struct __Pulse{
	UINT16	id;
	BOOL	isEnable;
	UINT16	phase;
	UINT16 	pin;
	VOID*	owner;
	GPIO_TypeDef* io;
	PULSE_CB 	  callback;
	TIM_HandleTypeDef timHandler;
}Pulse;
/********************************************************************
 * Pulse Method
 *******************************************************************/
VOID PulseInit (Pulse* pulse, PULSE_CB callback);
VOID PulseStart(Pulse* pulse);
VOID PulseStop (Pulse* pulse);
VOID PulseSetFreq(Pulse* pulse, UINT32 freq);
VOID PulseSetOwner(Pulse* pulse, VOID* owner);
Pulse* PulseGetObject(UINT16 id);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _PULSE_H_
