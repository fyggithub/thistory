/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#ifndef _CONFIGLIQUID_H_
#define _CONFIGLIQUID_H_

#include "Datatypes.h"
#include "Config.h"

#define CONST_LIQUID_CALCULATE_TEMPERATURE                     37 // 预热目标温度
#define CONST_LIQUID_PROPORTION_COEFFICIENT                     1 // PID的比例系数 
#define CONST_LIQUID_INTEGRAL_COEFFICIENT                       1 // PID的积分系数 
#define CONST_LIQUID_DIFFERENTIAL_COEFFICIENT                   1 // PID的微分系数 
#define CONST_LIQUID_EPS1                                       0 // 积分分离常数1
#define CONST_LIQUID_EPS2                                       0 // 积分分离常数2
#define CONST_LIQUID_UMAX                                       1 // PID差值最大值
#define CONST_LIQUID_UMIN                                       0 // PID差值最小值
#define CONST_LIQUID_REAGENT_COMPENSATION                       1 // 试剂温控补偿
#define CONST_LIQUID_CLEAN_COMPENSATION                         1 // 清洗温控补偿
#define CONST_LIQUID_REAGENT_OR_SAMPLE                          1 // 试剂或样本模式选择
#define CONST_LIQUID_REAGENT_COMPENSATION_TIME                  1 // 试剂开环补偿时间
#define CONST_LIQUID_REAGENT_COMPENSATION_COEFFICIENT           1 // 试剂开环补偿系数
#define CONST_LIQUID_CLEAN_COMPENSATION_TIME                    1 // 清洗开环补偿时间
#define CONST_LIQUID_CLEAN_COMPENSATION_COEFFICIENT             1 // 清洗开环补偿系数

enum{
	CFG_CFG_UNUSED                              = CFG_USER_BEGIN, // UNUSED
	CFG_MAX
};

extern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];

#endif
