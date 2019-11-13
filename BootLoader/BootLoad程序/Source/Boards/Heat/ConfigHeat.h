/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#ifndef _CONFIGHEAT_H_
#define _CONFIGHEAT_H_

#include "Datatypes.h"
#include "Config.h"

#define CONST_HEAT_CALCULATE_TEMPERATURE                       37 // 孵育测量预热理论目标温度
#define CONST_HEAT_SENSOR_REAGENT_ERROR                         0 // 传感器温度与试剂温度偏差
#define CONST_HEAT_SENSOR_REAGENT_DIFF_15                       0 // 传感器与试剂温度偏差-室温15度
#define CONST_HEAT_SENSOR_REAGENT_DIFF_30                       0 // 传感器与试剂温度偏差-室温30度
#define CONST_HEAT_PROPORTION_COEFFICIENT                     100 // PID的比例系数100倍数 
#define CONST_HEAT_INTEGRAL_COEFFICIENT                         0 // PID的积分系数100倍数 
#define CONST_HEAT_DIFFERENTIAL_COEFFICIENT                     0 // PID的微分系数100倍数 
#define CONST_HEAT_EPS1                                       100 // 积分分离常数1 100倍数
#define CONST_HEAT_EPS2                                        20 // 积分分离常数2 100倍数
#define CONST_HEAT_UMAX                                        70 // PID差值最大值100倍数
#define CONST_HEAT_UMIN                                         0 // PID差值最小值100倍数
#define CONST_HEAT_SENSOR_R0                                    1 // 传感器R0
#define CONST_HEAT_SENSOR_R37                                   1 // 传感器R37
#define CONST_HEAT_SENSOR_R100                                  1 // 传感器R100
#define CONST_HEAT_SENSOR_ADJUSTMENT_PARAMETERS1                1 // 传感器调整参数1
#define CONST_HEAT_SENSOR_ADJUSTMENT_PARAMETERS2                1 // 传感器调整参数2
#define CONST_HEAT_SENSOR_OPENCIRCUIT_TEMPERATURE               1 // 传感器短路温度
#define CONST_HEAT_SENSOR_SHORTCIRCUIT_TEMPERATURE              1 // 传感器断路温度

enum{
	CFG_CFG_UNUSED                              = CFG_USER_BEGIN, // UNUSED
	CFG_MAX
};

extern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];

#endif
