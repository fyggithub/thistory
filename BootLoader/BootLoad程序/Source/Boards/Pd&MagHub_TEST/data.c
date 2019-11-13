/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 * 
 * data.c
 *******************************************************************/
#include <stdio.h>
#include "Datatypes.h"
#include <string.h>
#include "data.h"
#include "LED.h"
/********************************************************************
 *
 *******************************************************************/
#define PD_DATA_SIZE	100
#define MAG_DATA_SIZE  300

/********************************************************************
 *
 *******************************************************************/
//Data PdData;
//Data MagData;
Data objData[2];
Data *pdDataPtr=&objData[0];
Data *magDataPtr=&objData[1];
UINT8 Pd2018[PD_DATA_SIZE*24];
UINT8 Mag2018[MAG_DATA_SIZE*2];

/********************************************************************
 *
 *******************************************************************/
VOID DataInit(VOID)
{
	pdDataPtr->counter=0;
	pdDataPtr->basesize=PD_DATA_SIZE;
	pdDataPtr->base=Pd2018;
    pdDataPtr->block=24;
    
	magDataPtr->counter=0;
	magDataPtr->basesize=MAG_DATA_SIZE;
	magDataPtr->base=Mag2018;
    magDataPtr->block=2;
}
/********************************************************************
 *
 *******************************************************************/
VOID DataAdd(UINT8 method,UINT8 *scr)
{
    UINT8 *dest=NULL;
    UINT16 x = 0;
		UINT8 data[24];
    for(UINT16 i=0;i<objData[method].block;i=i+2){
        data[i] = scr[i+1];
        data[i+1]=scr[i];
    }
	x = objData[method].counter % objData[method].basesize;
	dest = objData[method].base + x * objData[method].block;
	memcpy((BYTE*)dest,data,objData[method].block);
	(objData[method].counter)++;    
}
/********************************************************************
 *
 *******************************************************************/
VOID _DataPeek(UINT8 method,UINT8 *dest,UINT32 begin)
{
    UINT16 x =begin % objData[method].basesize;
    UINT8 *src = objData[method].base + x*objData[method].block;
    memcpy(dest,(BYTE*)src, objData[method].block);
}
/********************************************************************
 *
 *******************************************************************/
VOID DataReset(UINT8 method)
{
    objData[method].counter=0;
}
/********************************************************************
 *
 *******************************************************************/
UINT32 DataFirstTime(UINT8 method)
{
    UINT32 x = 0;
        x = objData[method].counter>objData[method].basesize ? 
            (objData[method].counter-objData[method].basesize) : 0;
    return x;  
}
/********************************************************************
 *
 *******************************************************************/
UINT32 DataLastTime(UINT8 method)
{
    UINT32 x = 0;
        x = objData[method].counter>objData[method].basesize ? 
			(objData[method].counter-1) : 0;
    return x;
}
/********************************************************************
 *
 *******************************************************************/
UINT8 DataPeek(UINT8 method,UINT8 *dest,UINT32 begin,UINT16 len)
{
    UINT32 temp;
    UINT8 *destPosition=NULL;
    UINT8* w = NULL;
    UINT8 RET = 3;

    if((begin+ objData[method].basesize)< objData[method].counter | 
        begin >= objData[method].counter){
        w = &dest[0];
        temp = 0x01F1F1;
        ARRAYSET(w,temp);    
        return RET;            
    }//error range
        
    dest[0] = 0;
    w = &dest[1];
    ARRAYSET(w,begin);
    len = (begin+len)>objData[method].counter ? (objData[method].counter-begin) : len;
    w = &dest[4];
    ARRAYSET(w,(begin+len-1));
    
    temp = begin; 
    switch(method){
        case MAG_METHOD:{
            dest[7] = 0x00;
            for(UINT8 i=0; i<len; i++){
                destPosition = &dest[8+i*2];  
                _DataPeek(MAG_METHOD,destPosition,temp);
                temp++;
            }
            RET = 2*len + 8;
            break;
        }
        case PD_METHOD:{         
           for(UINT8 i=0; i<len; i++){
                w = &dest[7+i*27];
                ARRAYSET(w,temp);
                destPosition = &dest[10+i*27];  
                _DataPeek(PD_METHOD,destPosition,temp);
                temp++;
            }
            RET = 27*len + 7;
            break;
        }
        default:
            break;
    }
    return RET;
}
/********************************************************************
 * END OF FILE
 *******************************************************************/

