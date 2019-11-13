/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#ifndef _CONFIGGRAB_H_
#define _CONFIGGRAB_H_

#include "Datatypes.h"
#include "Config.h"

#define CONST_GRAB_NSTEP_ACC_DOWN                              96 // 垂直下降加速台阶
#define CONST_GRAB_NSTEP_DEC_DOWN                              96 // 垂直下降减速台阶
#define CONST_GRAB_NSTEP_ACC_UP                                96 // 垂直上升加速台阶
#define CONST_GRAB_NSTEP_DEC_UP                                96 // 垂直上升减速台阶
#define CONST_GRAB_X_ACC_UP                                    96 // X轴水平加速台阶
#define CONST_GRAB_X_DEC_UP                                    96 // X轴水平减速台阶
#define CONST_GRAB_Y_ACC_UP                                    96 // Y轴水平加速台阶
#define CONST_GRAB_Y_DEC_UP                                    96 // Y轴水平减速台阶

enum{
	CFG_GRAB_POSLMT_VNEG                        = CFG_USER_BEGIN, // 垂直负向极限位置
	CFG_GRAB_POSLMT_VPOS                                        , // 垂直正向极限位置
	CFG_GRAB_POS_VRST                                           , // 垂直初始位置
	CFG_GRAB_MIX_HIGHT                                          , // 混匀高度垂直位置
	CFG_GRAB_DELTA_HIGHT                                        , // 放杯位置相对抓杯位置高度差
	CFG_GRAB_MESURE_CATCH_POS                                   , // 光学测量位抓杯垂直位置
	CFG_GRAB_MESURE_VERTICAL_LIMIT                              , // 光学测量位垂直极限位置
	CFG_GRAB_BORN_CATCH_POS                                     , // 光学孵育位抓杯垂直位置
	CFG_GRAB_BORN_VERTICAL_LIMIT                                , // 光学孵育位垂直极限位置
	CFG_GRAB_MAGNETIC_CATCH_POS                                 , // 磁珠杯手动区抓杯垂直位置
	CFG_GRAB_MAGNETIC_VERTICAL_LIMIT                            , // 磁珠杯手动区垂直极限位置
	CFG_GRAB_MAGMEA_CATCH_POS                                   , // 磁珠法测量位抓杯垂直位置
	CFG_GRAB_MAGMEA_VERTICAL_LIMIT                              , // 磁珠法测量位垂直极限位置
	CFG_GRAB_DISK_CATCH_POS                                     , // 调度盘4#抓杯垂直位置
	CFG_GRAB_DISK_VERTICAL_LIMIT                                , // 调度盘4#垂直极限位置
	CFG_GRAB_COOPERATE_SAMPLE_VERTICAL                          , // 配合样本针垂直位置
	CFG_GRAB_COOPERATE_SAMPLE_LIMIT                             , // 配合样本针垂直极限位置
	CFG_GRAB_COOPERATE_REAGENT_VERTICAL                         , // 配合试剂针垂直位置
	CFG_GRAB_COOPERATE_REAGENT_LIMIT                            , // 配合试剂针垂直极限位置
	CFG_GRAB_THROW_VERTICAL                                     , // 抛杯位置垂直位置
	CFG_GRAB_THROW_LIMIT                                        , // 抛杯位置垂直极限位置
	CFG_GRAB_X_NEG_LIMIT                                        , // X轴负向极限位置
	CFG_GRAB_X_POS_LIMIT                                        , // X轴正向极限位置
	CFG_GRAB_X_INIT_POS                                         , // X轴初始位置
	CFG_GRAB_Y_NEG_LIMIT                                        , // Y轴负向极限位置
	CFG_GRAB_Y_POS_LIMIT                                        , // Y轴正向极限位置
	CFG_GRAB_Y_INIT_POS                                         , // Y轴初始位置
	CFG_GRAB_CATCH_Y_DISTANCE                                   , // 抓杯准备位置Y轴相对距离
	CFG_GRAB_THROW_Y_DISTANCE                                   , // 抛杯完成位置Y轴相对距离
	CFG_GRAB_CORRECTE_POS1                                      , // 校正码齿1位置-X轴
	CFG_GRAB_CORRECTE_POS2                                      , // 校正码齿2位置-X轴
	CFG_GRAB_DISK_X_POS                                         , // 调度盘4#-X轴
	CFG_GRAB_DISK_Y_POS                                         , // 调度盘4#-Y轴
	CFG_GRAB_COOPERATE_SAMPLE_XPOS                              , // 配合样本针加样-X轴
	CFG_GRAB_COOPERATE_SAMPLE_YPOS                              , // 配合样本针加样-Y轴
	CFG_GRAB_COOPERATE_REAGENT_XPOS                             , // 配合试剂针加样-X轴
	CFG_GRAB_COOPERATE_REAGENT_YPOS                             , // 配合试剂针加样-Y轴
	CFG_GRAB_THROW_X_POS1                                       , // 抛杯位1-X轴
	CFG_GRAB_THROW_Y_POS1                                       , // 抛杯位1-Y轴
	CFG_GRAB_THROW_X_POS2                                       , // 抛杯位2-X轴
	CFG_GRAB_THROW_Y_POS2                                       , // 抛杯位2-Y轴
	CFG_GRAB_MAGMEA_X_POS                                       , // 磁珠法测量位置51-X轴
	CFG_GRAB_MEGMEA_Y_POS                                       , // 磁珠法测量位置51-Y轴
	CFG_GRAB_MESURE_X_POS1                                      , // 光学测量位置1-X轴
	CFG_GRAB_MESURE_Y_POS1                                      , // 光学测量位置1-Y轴
	CFG_GRAB_MESURE_X_POS12                                     , // 光学测量位置12-X轴
	CFG_GRAB_MESURE_Y_POS12                                     , // 光学测量位置12-Y轴
	CFG_GRAB_BORN_X_POS21                                       , // 光学孵育位21-X轴
	CFG_GRAB_BORN_Y_POS21                                       , // 光学孵育位21-Y轴
	CFG_GRAB_BORN_X_POS26                                       , // 光学孵育位26-X轴
	CFG_GRAB_BORN_Y_POS26                                       , // 光学孵育位26-Y轴
	CFG_GRAB_BORN_X_POS38                                       , // 光学孵育位38-X轴
	CFG_GRAB_BORN_Y_POS38                                       , // 光学孵育位38-Y轴
	CFG_GRAB_MEGNETIC_X_POS61                                   , // 磁珠杯手动放杯61--X轴
	CFG_GRAB_MEGNETIC_Y_POS61                                   , // 磁珠杯手动放杯61--Y轴
	CFG_GRAB_MEGNETIC_X_POS72                                   , // 磁珠杯手动放杯72--X轴
	CFG_GRAB_MEGNETIC_Y_POS72                                   , // 磁珠杯手动放杯72--Y轴
	CFG_GRAB_MIX_STRENGTH1                                      , // 混匀强度1-光学法-弱
	CFG_GRAB_MIX_STRENGTH2                                      , // 混匀强度2-光学法-中弱
	CFG_GRAB_MIX_STRENGTH3                                      , // 混匀强度3-光学法-中
	CFG_GRAB_MIX_STRENGTH4                                      , // 混匀强度4-光学法-中强
	CFG_GRAB_MIX_STRENGTH5                                      , // 混匀强度5-光学法-强
	CFG_GRAB_MIX_STRENGTH6                                      , // 混匀强度6-磁珠法-弱
	CFG_GRAB_MIX_STRENGTH7                                      , // 混匀强度7-磁珠法-中弱
	CFG_GRAB_MIX_STRENGTH8                                      , // 混匀强度8-磁珠法-中
	CFG_GRAB_MIX_STRENGTH9                                      , // 混匀强度9-磁珠法-中强
	CFG_GRAB_MIX_STRENGTH10                                     , // 混匀强度10-磁珠法-强
	CFG_GRAB_OPTICS_MIX_SAMPLE_TIME                             , // 光学混匀时间-样本混合液
	CFG_GRAB_OPTICS_MIX_MIX_TIME                                , // 光学混匀时间-中间试剂
	CFG_GRAB_OPTICS_MIX_TRI_TIME                                , // 光学混匀时间-触发试剂
	CFG_GRAB_MEGNETIC_MIX_SAMPLE_TIME                           , // 磁珠混匀时间-样本混合液
	CFG_GRAB_MEGNETIC_MIX_MID_TIME                              , // 磁珠混匀时间-中间试剂
	CFG_GRAB_MEGNETIC_MIX_TRI_TIME                              , // 磁珠混匀时间-触发试剂
	CFG_GRAB_CATCH_CORRECTE                                     , // 是否开启抓手X轴校正
	CFG_GRAB_MIX_ACC_UP                                         , // 混匀速度加速台阶
	CFG_GRAB_MIX_DEC_UP                                         , // 混匀速度减速台阶
	CFG_GRAB_MESURE_CATCH_POS12                                 , // 光学测量位抓杯垂直位置12
	CFG_GRAB_BORN_CATCH_POS26                                   , // 光学孵育位抓杯垂直位置26
	CFG_GRAB_BORN_CATCH_POS38                                   , // 光学孵育位抓杯垂直位置38
	CFG_GRAB_MAGNETIC_CATCH_POS72                               , // 磁珠杯手动区抓杯垂直位置72
	CFG_MAX
};

extern CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN];

#endif
