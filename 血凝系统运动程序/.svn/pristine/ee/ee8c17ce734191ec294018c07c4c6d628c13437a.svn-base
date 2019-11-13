/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * TLC2543.c
 *******************************************************************/
#include "TLC2543.h"
#include "Console.h"
#include "Bits.h"
#include "spi.h"
#include "Board.h"
/********************************************************************
 *TLC_VOL2TEM = 1000000.0 / TLC_RES / TLC_GAIN = 16.507 mv/C
 *TLC_GAIN=157.329114
 *TLC_RES=(1385.055 - 1000)
 *******************************************************************/
#define PD_TEMP_AD_CHANNEL  0x2000
#define TLC_CONFIG          0x0C00
#define TLC_NUM2VOL         (4.092/4092)
#define TLC_VOL2TEM         16.507  
/********************************************************************
 *
 *******************************************************************/
struct _tempRem{
    DOUBLE temp3Mid[3];
    UINT16 temp3MidId;
    DOUBLE temp5Mean[5];
    UINT16 temp5MeanId;
}tempRem;
/********************************************************************
 *
 *******************************************************************/ 
DOUBLE _TempSampleRead(VOID)
{
	UINT16 data;
    UINT16 temp=0;
    DOUBLE temp2=0;
    UINT16 config = PD_TEMP_AD_CHANNEL | TLC_CONFIG;
    SpiSendRecv(BoardGetSpiExt(),(BYTE*)&config,(BYTE*)&data,1);
    temp = data>>4;
    temp2 = temp * TLC_NUM2VOL * TLC_VOL2TEM;
    return temp2;
}


/********************************************************************
 *
 *******************************************************************/
DOUBLE TempSampleRead_mid(VOID)
{
    UINT16 i=tempRem.temp3MidId;
    DOUBLE mid=0;
    tempRem.temp3Mid[i]=_TempSampleRead();
    tempRem.temp3MidId=(tempRem.temp3MidId+1)%3;
    DOUBLE a=tempRem.temp3Mid[0];
    DOUBLE b=tempRem.temp3Mid[1];
    DOUBLE c=tempRem.temp3Mid[2];
    mid=a < b ? (b < c ? b : a < c ? c : a) : (b > c ? b : a > c ? c: a);
    return mid;
}
/********************************************************************
 *
 *******************************************************************/
DOUBLE TempSampleRead(VOID)
{
    UINT16 i=tempRem.temp5MeanId;
    DOUBLE sum=0;
    tempRem.temp5Mean[i]=TempSampleRead_mid();
    tempRem.temp5MeanId=(tempRem.temp5MeanId+1)%5;
    for(UINT16 j=0;j<5;j++)
        sum+=tempRem.temp5Mean[j];
    return sum/5;
}
/********************************************************************
 *
 *******************************************************************/
VOID TLCInit(VOID)
{
    UINT16 i=0;
    tempRem.temp3MidId=0;
    tempRem.temp5MeanId=0;
    _TempSampleRead();//The first data TLC2543 accessed is inaccurate.
    for(i=0;i<20;i++){
        TempSampleRead();
    }
}
/********************************************************************
 *
 *******************************************************************/
