/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * AD7689.c
 *******************************************************************/
#include "AD7689.h"
#include "Board.h"
#include "delayus.h"

#define US10 10
#define CH_CNV_HIGH(IO_CNV_ADC)   GpioSet(BoardGetGpio(IO_CNV_ADC),1)
#define CH_CNV_LOW(IO_CNV_ADC)  GpioSet(BoardGetGpio(IO_CNV_ADC),0)
/********************************************************************
 *
 *******************************************************************/
STATIC AD7689 ADCh1Ch6;
STATIC AD7689 ADCh7Ch12;
/********************************************************************
 *
 *******************************************************************/
VOID AD7689RegConfig(AD7689* _AD7689Def,Spi*_spi)
{
    UINT16 cfg=_AD7689Def->ResConfig;
    UINT16 SetSpiMOSIToLow=0x0000;
    DelayUS(US10);
    CH_CNV_LOW(_AD7689Def->IoCnv);
    DelayUS(US10);
    SpiSend(_spi,(UINT8*)&cfg, 1);
    DelayUS(US10);
    CH_CNV_HIGH(_AD7689Def->IoCnv);
    DelayUS(US10);
   // SpiSend(_spi,(UINT8*)&SetSpiMOSIToLow, 1);
   // SpiRecv(_spi,(UINT8*)&SetSpiMOSIToLow, 1);
    return;
}
/********************************************************************
 *
 *******************************************************************/
VOID DataRead(AD7689* _AD7689Def,Spi*_spi,UINT16 len)
{
    BYTE* ptemp =(BYTE*)_AD7689Def->temp;  
    UINT16 SetSpiMOSIToLow=0x0000;
    for(UINT8 i=0;i<len;i++){
        _AD7689Def->temp[i] = 0x0000;
        CH_CNV_HIGH(_AD7689Def->IoCnv);
        DelayUS(US10);
        CH_CNV_LOW(_AD7689Def->IoCnv);
        DelayUS(US10);
        SpiRecv(_spi, ptemp, 1);
        //SpiSendRecv(_spi, (BYTE*) &SetSpiMOSIToLow,ptemp, 1);
        CH_CNV_HIGH(_AD7689Def->IoCnv);
		DelayUS(US10*5);
        ptemp=ptemp+2;
    }
     for(UINT8 i=0;i<6;i++){
		if(i==0)       _AD7689Def->BuffPtr[3]=_AD7689Def->temp[i];
		else if(i==1)  _AD7689Def->BuffPtr[2]=_AD7689Def->temp[i];	
		else if(i==2)  _AD7689Def->BuffPtr[1]=_AD7689Def->temp[i];
		else if(i==3)  _AD7689Def->BuffPtr[0]=_AD7689Def->temp[i];
		else if(i==4)  _AD7689Def->BuffPtr[4]=_AD7689Def->temp[i];
		else if(i==5)  _AD7689Def->BuffPtr[5]=_AD7689Def->temp[i];
	}//PCB channel is not eaqual to AD7689 channel
    return;
}
/********************************************************************
 *
 *******************************************************************/
VOID AD7689Get(UINT16* ch)
{
    ADCh1Ch6.ResConfig =0x3EF7<<2;
    ADCh1Ch6.IoCnv=IO_CNV_ADC1;
    ADCh1Ch6.BuffPtr=ch;
    AD7689RegConfig(&ADCh1Ch6,BoardGetSpiExt());
    DataRead(&ADCh1Ch6,BoardGetSpiExt(),1);//无效数据
    DataRead(&ADCh1Ch6,BoardGetSpiExt(),6);
    
    ADCh7Ch12.ResConfig=0x3EF7<<2;
    ADCh7Ch12.IoCnv=IO_CNV_ADC2;
    ADCh7Ch12.BuffPtr=ch+6;
    AD7689RegConfig(&ADCh7Ch12,BoardGetSpiExt());
    DataRead(&ADCh7Ch12,BoardGetSpiExt(),1);
    DataRead(&ADCh7Ch12,BoardGetSpiExt(),6);
}
/********************************************************************
 * End of File
 *******************************************************************/

