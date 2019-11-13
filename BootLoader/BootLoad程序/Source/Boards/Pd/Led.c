/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * led.c
 *******************************************************************/
#include "led.h"

struct _led{
    Gpio* ledGpio[gpioNumMax];
}Led;
VOID LedInit(VOID)
{
    for(UINT8 i=0; i<6 ;i++){
        Led.ledGpio[i]=BoardGetGpio(IO_LEDCON1+i);
        GpioSet(Led.ledGpio[i],LEDOFF);
    }    
}
/********************************************************************
 *
 *******************************************************************/
VOID LedSwitch(BOOL state)
{
    if(state==LEDON){
        for(UINT8 i=0; i<6; i++){
            GpioSet(Led.ledGpio[i],LEDON);
        }
    }
    else{
        for(UINT8 i=0; i<6; i++){
            GpioSet(Led.ledGpio[i],LEDOFF);
        }
    }
}

/********************************************************************
 * End of File
 *******************************************************************/

