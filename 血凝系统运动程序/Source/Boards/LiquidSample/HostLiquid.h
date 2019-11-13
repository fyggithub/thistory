/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostLiquid.h
 *******************************************************************/
#ifndef HOSTLIQUID_H
#define HOSTLIQUID_H
#include "Datatypes.h"
#define HOST_ADDR_LIQUID	(0x08)

#define HOST_CMD_VERSION	0xC1
#define HOST_CMD_GETINFO	0xC2
#define HOST_CMD_SETINFO	0xC3
#define HOST_CMD_PARAINFO	0xC4
#define HOST_CMD_PARAMETER_SET      (0xC8)
#define HOST_CMD_TEMPERATURE_ASK	(0xC9)
#define HOST_CMD_PWM_SWITCH         (0xCA)
VOID HostLiquidInit(VOID);
VOID HostLiquidProcess(VOID);
#endif

/********************************************************************
* END
*******************************************************************/
