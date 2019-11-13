#ifndef __stateReady_H__
#define __stateReady_H__

#include "Console.h"
#include "Datatypes.h"
#include "Message.h"
#include "Timer.h"
#include "StepMotor.h"
#include "Regent.h"
#include "Board.h"
#include "SendResultCode.h"
#include "LevelDetection.h"
#include "ConfigReage.h"


#define REAGENT_SUBDAR_SPEED        		 15     //注射电机吸排液的速度
#define REAGENT_X_YLINKAGEMOTOR1_SPEED       15     //样本针X轴电机是从哪个速度台阶来运行的
#define REAGENT_X_YLINKAGEMOTOR2_SPEED       15     //样本针Y轴电机是从哪个速度台阶来运行的
#define REAGENT_ZAXISSUBSAM_RISESPEED        15     //样本针下降-吸试剂-上升  Z轴上升速度 
#define REAGENT_ZAXISSUBSAM_FALLSPEED        15      //样本针下降-吸试剂-上升  Z轴下降速度 
#define REAGENT_INJSUBSAM_SPEED              15     //样本针下降-吸试剂-上升  吸液的速度
#define REAGENT_ZAXISDRAIN_RISESPEED         15     //样本针下降-排样-上升 Z轴上升速度
#define REAGENT_ZAXISDRAIN_FALLSPEED         15      //样本针下降-排样-上升 Z轴下降速度
#define REAGENT_INJDRAIN_SPEED               15     //样本针下降-排样-上升 排液的速度
#define REAGENT_ZAXISDRAINCLE_RISESPEED      15     //样本针下降-清洗-上升  Z轴上升速度
#define REAGENT_ZAXISDRAINCLE_FALLSPEED      15      //样本针下降-清洗-上升  Z轴下降速度
#define REAGENT_INJDRAINCLE_SPEED            15     //样本针下降-清洗-上升  吸排液的速度

//1 - 负脉冲个数远离光耦  0 - 正脉冲个数远离光耦
#define MOTOR_X_POSITIVE_DIRECTION 		1     
#define MOTOR_Y_POSITIVE_DIRECTION 		0
#define MOTOR_Z_POSITIVE_DIRECTION 		1
#define MOTOR_SYN_POSITIVE_DIRECTION 	1

VOID OpenLiquid(VOID);
VOID CloseLiquid(VOID);
VOID StepMotorSubAirCallback(VOID);
VOID RegeSubAirPluseNum(VOID);
INT32 DirSortOut(INT16 motor,INT16 step,INT16 dir);

VOID RegeStateEndHandler(VOID);
VOID RegeStateBasicCmdMessageHandler(Message* msg);
VOID RegeStateReadyMessageHandler(Message* msg);
VOID RegeStateMotorZaixDetMessageHandler(Message* msg);



#endif

