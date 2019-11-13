/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * StateMonitor.h
 *******************************************************************/
#ifndef _STATEMONITOR_H
#define _STATEMONITOR_H

#include "Datatypes.h"
#include "Message.h"
#define VERSION_XX	1
#define VERSION_YY	0
#define VERSION_ZZ	3
typedef struct _MonitorApp{
	UINT8  tag;
	UINT8  mSam[8];
	UINT8  mSamSum;
	BOOL   mIsCleanEmpty;
	BOOL   mIsWasteBarryFull;
	BOOL   mIsDoorOpen;
	BOOL   mIsEmergency;
}MonitorApp;

enum LEDMETHOD{
	SHUTDOWN = 0,
	START,
	SLEEP,
	STANDBY,
	MAINTAIN,
	PROCESS,
	PAUSE,
	ABEND,
	LEDMETHOD_TEST,
};

UINT8 GetSam1GPIO(VOID);
UINT8 GetSam2GPIO(VOID);
VOID SetSam1Leds(VOID);
VOID CurrentState(UINT8* data,UINT8 len);
BOOL GetDoorState(VOID);
BOOL GetCleanState(VOID);
BOOL GetWasteBarryState(VOID);
VOID StateMonitorInit(VOID);
VOID EmergencyStopPoll(VOID);
/********************************************************************
 * End of File
*******************************************************************/
#endif
