/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Adc.h
 *******************************************************************/
#ifndef _ADC_H_
#define _ADC_H_
#include "Datatypes.h"
#include "Gpio.h"
/********************************************************************
 *
 *******************************************************************/
VOID AdcStart(VOID);
extern UINT32 AdcValue[3];
extern ADC_HandleTypeDef hadcx;
BOOL IsLiquidContact(VOID);
UINT16 ReadNtcValueUint16(VOID);
DOUBLE ReadNtcValue(VOID);
DOUBLE ReadLqAdjValue(VOID);
DOUBLE ReadLqValue(VOID);
VOID AdcValueInit(VOID);
DOUBLE ReadNtcVol(VOID);
VOID Timer4Start(VOID);
 #endif // _ADC_H_
/********************************************************************
 * End of File
 *******************************************************************/
