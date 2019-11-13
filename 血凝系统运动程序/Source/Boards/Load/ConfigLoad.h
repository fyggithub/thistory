/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#ifndef _CONFIGLOAD_H_
#define _CONFIGLOAD_H_

#include "Datatypes.h"
#include "Config.h"

#define CONST_LOAD_NSTEP_ACC_DOWN                              96 // 垂直下降加速台阶
#define CONST_LOAD_NSTEP_DEC_DOWN                              96 // 垂直下降减速台阶
#define CONST_LOAD_NSTEP_ACC_UP                                96 // 垂直上升加速台阶
#define CONST_LOAD_NSTEP_DEC_UP                                96 // 垂直上升减速台阶
#define CONST_LOAD_NSTEP_ACC_ROTATE                            96 // 转动加速台阶
#define CONST_LOAD_NSTEP_DEC_ROTATE                            96 // 转动减速台阶

enum{
	CFG_LOAD_POSLMT_VNEG                        = CFG_USER_BEGIN, // 垂直负向极限位置
	CFG_LOAD_POSLMT_VPOS                                        , // 垂直正向极限位置
	CFG_LOAD_POS_VRST                                           , // 垂直初始位置
	CFG_LOAD_POS_VTOP                                           , // 装杯过程向上停止位置
	CFG_LOAD_POS_VBOT                                           , // 装杯过程向下停止位置
	CFG_LOAD_POS_HRST                                           , // 复位位置-调度盘
	CFG_LOAD_POS_WRK1                                           , // 1#孔位对准1#盘位的位置
	CFG_LOAD_POS_WRK2                                           , // 1#孔位对准2#盘位的位置
	CFG_LOAD_POS_WRK3                                           , // 1#孔位对准3#盘位的位置
	CFG_LOAD_POS_WRK4                                           , // 1#孔位对准4#盘位的位置
	CFG_LOAD_DELTA_HOLE2                                        , // 2#孔位位置与理论位置偏差
	CFG_LOAD_DELTA_HOLE3                                        , // 3#孔位位置与理论位置偏差
	CFG_LOAD_DELTA_HOLE4                                        , // 4#孔位位置与理论位置偏差
	CFG_MAX
};

extern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];

#endif
