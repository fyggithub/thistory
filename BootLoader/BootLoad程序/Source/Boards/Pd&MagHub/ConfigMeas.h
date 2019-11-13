/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#ifndef _CONFIGMEAS_H_
#define _CONFIGMEAS_H_

#include "Datatypes.h"
#include "Config.h"

#define CONST_HUB_LIGHT_STABLE_TIME                            20 // 光源稳定时间

enum{
	CFG_CFG_UNUSED                              = CFG_USER_BEGIN, // UNUSED
	CFG_MAX
};

extern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];

#endif
