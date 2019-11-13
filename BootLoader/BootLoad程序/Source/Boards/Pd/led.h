#ifndef _LED_H
#define _LED_H
#include "Gpio.h"
#include "Board.h"
#include "Datatypes.h"
#define gpioNumMax 6
#define LEDOFF 1
#define LEDON  0
VOID LedSwitch(BOOL state);
VOID LedInit(VOID);
#endif
