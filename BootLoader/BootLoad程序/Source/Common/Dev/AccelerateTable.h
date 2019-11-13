/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * AccelerateTable.h
 *******************************************************************/
#ifndef _ACCELERATE_TABLE_H_
#define _ACCELERATE_TABLE_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
enum{
	ACCTBL_REAGENT_X=0, ACCTBL_REAGENT_Y,  ACCTBL_REAGENT_Z_UP,  ACCTBL_REAGENT_Z_DOWN, ACCTBL_REAGENT_SYN_UP,  ACCTBL_REAGENT_SYN_DOWN,
	ACCTBL_SAMPLE_X,    ACCTBL_SAMPLE_Y,   ACCTBL_SAMPLE_Z_UP,   ACCTBL_SAMPLE_Z_DOWN,	ACCTBL_SAMPLE_SYN_UP,   ACCTBL_SAMPLE_SYN_DOWN,
	ACCTBL_GRAB_X,      ACCTBL_GRAB_Y,     ACCTBL_GRAB_Z_UP,     ACCTBL_GRAB_Z_DOWN,
	ACCTBL_LOAD_X,      ACCTBL_DISK_X,     ACCTBL_MAX,
};
typedef struct __AccTbl{
	UINT16 stepInterval[32];
	UINT32 freqTbl[32];
}AccTbl;
/********************************************************************
 *
 *******************************************************************/
CONST AccTbl* GetAccelerateTable(UINT16 id);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _ACCELERATE_TABLE_H_

