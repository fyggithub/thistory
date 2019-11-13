#ifndef __stateReady_H__
#define __stateReady_H__

#include "Console.h"
#include "Datatypes.h"
#include "Message.h"
#include "Timer.h"
#include "StepMotor.h"
#include "Sample.h"
#include "Board.h"
#include "StepMotorSample.h"

#include "stateReset.h"
#include "stateReady.h"
#include "stateSub.h"
#include "stateDrain.h"
#include "stateMoveWork.h"
#include "stateClean.h"
#include "SendResultCode.h"
#include "LevelDetection.h"

#define SAMPLE_SUBDAR_SPEED        		 	15     //注射电机吸排液的速度
#define SAMPLE_X_YLINKAGEMOTOR1_SPEED       15     //样本针X轴电机是从哪个速度台阶来运行的
#define SAMPLE_X_YLINKAGEMOTOR2_SPEED       15     //样本针Y轴电机是从哪个速度台阶来运行的
#define SAMPLE_ZAXISSUBSAM_RISESPEED        15     //样本针下降-吸试剂-上升  Z轴上升速度 
#define SAMPLE_ZAXISSUBSAM_FALLSPEED        15     //样本针下降-吸试剂-上升  Z轴下降速度 
#define SAMPLE_INJSUBSAM_SPEED              15     //样本针下降-吸试剂-上升  吸液的速度
#define SAMPLE_ZAXISDRAIN_RISESPEED         15     //样本针下降-排样-上升 Z轴上升速度
#define SAMPLE_ZAXISDRAIN_FALLSPEED         15     //样本针下降-排样-上升 Z轴下降速度
#define SAMPLE_INJDRAIN_SPEED               15      //样本针下降-排样-上升 排液的速度
#define SAMPLE_ZAXISDRAINCLE_RISESPEED      15     //样本针下降-清洗-上升  Z轴上升速度
#define SAMPLE_ZAXISDRAINCLE_FALLSPEED      15     //样本针下降-清洗-上升  Z轴下降速度
#define SAMPLE_INJDRAINCLE_SPEED            15      //样本针下降-清洗-上升  吸排液的速度

#define MOTOR_X_POSITIVE_DIRECTION 		1
#define MOTOR_Y_POSITIVE_DIRECTION 		0
#define MOTOR_Z_POSITIVE_DIRECTION 		1
#define MOTOR_SYN_POSITIVE_DIRECTION 	1

VOID OpenLiquid(VOID);
VOID CloseLiquid(VOID);
VOID SampleStateEndHandler(VOID);
VOID SampleStateBasicCmdMessageHandler(Message* msg);
VOID SampleStateMvMessageHandler(Message* msg);
VOID SampleStateSubMessageHandler(Message* msg);
VOID SampleStateDraMessageHandler(Message* msg);
VOID SampleStateClnMessageHandler(Message* msg);
VOID SampleStateReadyMessageHandler(Message* msg);

VOID StepMotorSubAirCallback(VOID);
VOID SampleSubAirPluseNum(VOID);

INT32 DirSortOut(INT16 motor,INT16 step,INT16 dir);
VOID SampleStateMotorZaixDetMessageHandler(Message* msg);

#endif



