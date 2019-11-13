/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * SlaveHub.h
 *******************************************************************/
#ifndef _SLAVE_HUB_H
#define	_SLAVE_HUB_H
#include "Datatypes.h"
#include "Uart.h"
#include "BoardHost.h"

#define SLAVENUM                    (2)
//#define PD                          (0)
//#define MAG                         (1)
/********************************************************************
 *
 *******************************************************************/
VOID SlaveSend(UINT8 id,BYTE* data, UINT16 len);
VOID SlaveInit(VOID);
VOID SlaveDataProcess(VOID);

#endif
/********************************************************************
 * End of File
 *******************************************************************/


