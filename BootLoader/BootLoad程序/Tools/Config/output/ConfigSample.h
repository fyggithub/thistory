/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#ifndef _CONFIGSAMPLE_H_
#define _CONFIGSAMPLE_H_

#include "Datatypes.h"
#include "Config.h"

#define CONST_SAMPLE_NSTEP_ACC_DOWN                            48 // 垂直下降加速台阶
#define CONST_SAMPLE_NSTEP_DEC_DOWN                            48 // 垂直下降减速台阶
#define CONST_SAMPLE_NSTEP_ACC_UP                              48 // 垂直上升加速台阶
#define CONST_SAMPLE_NSTEP_DEC_UP                              48 // 垂直上升减速台阶
#define CONST_SAMPLE_X_ACC_UP                                  96 // X轴水平加速台阶
#define CONST_SAMPLE_X_DEC_UP                                  96 // X轴水平减速台阶
#define CONST_SAMPLE_Y_ACC_UP                                  96 // Y轴水平加速台阶
#define CONST_SAMPLE_Y_DEC_UP                                  96 // Y轴水平减速台阶
#define CONST_SAMPLE_POSSTEP_SYRINGE_ACC                       96 // 注射器正向加速台阶
#define CONST_SAMPLE_POSSTEP_SYRINGE_DEC                       96 // 注射器正向减速台阶
#define CONST_SAMPLE_NEGSTEP_SYRINGE_ACC                       96 // 注射器负向加速台阶
#define CONST_SAMPLE_NEGSTEP_SYRINGE_DEC                       96 // 注射器负向减速台阶

enum{
	CFG_SAMPLE_POSLMT_VNEG                      = CFG_USER_BEGIN, // 垂直负向极限位置
	CFG_SAMPLE_POSLMT_VPOS                                      , // 垂直正向极限位置
	CFG_SAMPLE_POS_VRST                                         , // 垂直初始位置
	CFG_SAMPLE_CLEAN_UP_POS                                     , // 清洗池上方垂直位置
	CFG_SAMPLE_CLEAN_CLN_POS                                    , // 清洗池清洗垂直位置
	CFG_SAMPLE_CLEAN_LIMIT_POS                                  , // 清洗池垂直极限位置
	CFG_SAMPLE_ROOM_UP_POS                                      , // 常温试剂区域上方垂直位置
	CFG_SAMPLE_ROOM_VERTICAL_POS                                , // 常温试剂区域垂直极限位置
	CFG_SAMPLE_COOL_UP_POS                                      , // 冷藏试剂区域上方垂直位置
	CFG_SAMPLE_COOL_VERTICAL_LIMIT                              , // 冷藏试剂区域垂直极限位置
	CFG_SAMPLE_DISK_UP_POS                                      , // 调度盘2#位上方垂直位置
	CFG_SAMPLE_DISK_LOAD_POS                                    , // 调度盘2#位加样垂直位置
	CFG_SAMPLE_DISK_VERTICAL_LIMIT                              , // 调度盘2#位垂直极限位置
	CFG_SAMPLE_COOPERATE_UP_POS                                 , // 配合抓杯手上方垂直位置
	CFG_SAMPLE_COOPERATE_VERTICAL_POS                           , // 配合抓杯手垂直位置
	CFG_SAMPLE_COOPERATE_VERTICAL_LIMIT                         , // 配合抓杯手垂直极限位置
	CFG_SAMPLE_SAMPLEAREA_UP_POS                                , // 样本管区域上方垂直位置
	CFG_SAMPLE_SAMPLEAREA_VERTICAL_POS                          , // 样本管区域垂直极限位置
	CFG_SAMPLE_X_NEG_LIMIT                                      , // X轴负向极限位置
	CFG_SAMPLE_X_POS_LIMIT                                      , // X轴正向极限位置
	CFG_SAMPLE_X_INIT_POS                                       , // X轴初始位置
	CFG_SAMPLE_Y_NEG_LIMIT                                      , // Y轴负向极限位置
	CFG_SAMPLE_Y_POS_LIMIT                                      , // Y轴正向极限位置
	CFG_SAMPLE_Y_INIT_POS                                       , // Y轴初始位置
	CFG_SAMPLE_CLEAN_X_POS                                      , // 清洗池位置-X轴
	CFG_SAMPLE_ROOM_X_POS1                                      , // 常温试剂调试位1-X轴
	CFG_SAMPLE_ROOM_X_POS2                                      , // 常温试剂调试位2-X轴
	CFG_SAMPLE_COOL_X_POS1                                      , // 冷藏试剂调试位1-X轴
	CFG_SAMPLE_COOL_X_POS2                                      , // 冷藏试剂调试位2-X轴
	CFG_SAMPLE_DISK_X_POS                                       , // 调度盘2#-X轴
	CFG_SAMPLE_COOPERATE_X_POS                                  , // 配合抓杯手-X轴
	CFG_SAMPLE_SAMPLEAREA_X_POS1                                , // 样本管区域调试位置1-X轴
	CFG_SAMPLE_SAMPLEAREA_X_POS2                                , // 样本管区域调试位置2-X轴
	CFG_SAMPLE_SAMPLEAREA_X_POS3                                , // 样本管区域调试位置3-X轴
	CFG_SAMPLE_CLEAN_Y_POS                                      , // 清洗池位置-Y轴
	CFG_SAMPLE_ROOM_Y_POS1                                      , // 常温试剂调试位1-Y轴
	CFG_SAMPLE_ROOM_Y_POS2                                      , // 常温试剂调试位2-Y轴
	CFG_SAMPLE_COOL_Y_POS1                                      , // 冷藏试剂调试位1-Y轴
	CFG_SAMPLE_COOL_Y_POS2                                      , // 冷藏试剂调试位2-Y轴
	CFG_SAMPLE_DISK_Y_POS                                       , // 调度盘2#-Y轴
	CFG_SAMPLE_COOPERATE_Y_POS                                  , // 配合抓杯手-Y轴
	CFG_SAMPLE_SAMPLEAREA_Y_POS1                                , // 样本管区域调试位置1-Y轴
	CFG_SAMPLE_SAMPLEAREA_Y_POS2                                , // 样本管区域调试位置2-Y轴
	CFG_SAMPLE_SAMPLEAREA_Y_POS3                                , // 样本管区域调试位置3-Y轴
	CFG_SAMPLE_INNER_TIME                                       , // 内壁清洗时间
	CFG_SAMPLE_OUTTER_TIME                                      , // 外壁清洗时间
	CFG_SAMPLE_SUB_TIME                                         , // 吸液时间
	CFG_SAMPLE_DRAIN_TIME                                       , // 排液时间
	CFG_SAMPLE_SUB_FRONT_AIR                                    , // 吸液前吸空气量
	CFG_SAMPLE_ADD_SUB                                          , // 多吸液量
	CFG_SAMPLE_SUB_PUSH                                         , // 吸液后回推量
	CFG_SAMPLE_SUB_LATER_AIR                                    , // 吸液后吸空气量
	CFG_SAMPLE_DRAIN_LATER_AIR                                  , // 排液后吸空气量
	CFG_SAMPLE_CLEAN_LATER_AIR                                  , // 清洗后吸空气量
	CFG_SAMPLE_SYRINGE_NEGLMT                                   , // 注射器负向极限位置
	CFG_SAMPLE_SYRINGE_INIT_POS                                 , // 注射器初始位置
	CFG_SAMPLE_SYRINGE_POSLMT                                   , // 注射器正向极限位置
	CFG_SAMPLE_STEP_UL                                          , // 1微升对应微步数
	CFG_SAMPLE_NEEDLE_DETECT1                                   , // 堵针检测阈值1
	CFG_SAMPLE_NEEDLE_DETECT2                                   , // 堵针检测阈值2
	CFG_SAMPLE_NEEDLE_DETECT3                                   , // 堵针检测阈值3
	CFG_SAMPLE_NEEDLE_DETECT4                                   , // 堵针检测阈值4
	CFG_SAMPLE_NEEDLE_DETECT5                                   , // 堵针检测阈值5
	CFG_SAMPLE_LIQUID_LEVEL_STEP                                , // 检测到液面后Z轴下降步数
	CFG_MAX
};

extern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];

#endif
