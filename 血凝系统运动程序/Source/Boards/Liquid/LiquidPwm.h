/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * LiquidPwm.c
 *******************************************************************/
#ifndef LIQUID_PWM_H
#define LIQUID_PWM_H
#include "Board.h"
#include "stm32f1xx_hal.h"
#include "Gpio.h"
VOID PwmInit(VOID);
VOID PwmSetDutyRatio(DOUBLE ratio);
#endif

/********************************************************************
 *
 *******************************************************************/
