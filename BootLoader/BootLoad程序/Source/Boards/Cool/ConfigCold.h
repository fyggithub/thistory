/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#ifndef _CONFIGCOLD_H_
#define _CONFIGCOLD_H_

#include "Datatypes.h"
#include "Config.h"

#define CONST_COLD_CALCULATE_TEMPERATURE                       16 // 制冷目标温度
#define CONST_COLD_PROPORTION_COEFFICIENT                       1 // PID的比例系数 
#define CONST_COLD_INTEGRAL_COEFFICIENT                        10 // PID的积分系数 
#define CONST_COLD_DIFFERENTIAL_COEFFICIENT                     0 // PID的微分系数 
#define CONST_COLD_EPS1                                       200 // 积分分离常数1
#define CONST_COLD_EPS2                                       500 // 积分分离常数2
#define CONST_COLD_UMAX                                       500 // PID差值最大值
#define CONST_COLD_UMIN                                         0 // PID差值最小值
#define CONST_COLD_INTEGRAL_ITEMS_MAX                         500 // 积分项最大值

enum{
	CFG_CFG_UNUSED                              = CFG_USER_BEGIN, // UNUSED
	CFG_MAX
};

extern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];

#endif
