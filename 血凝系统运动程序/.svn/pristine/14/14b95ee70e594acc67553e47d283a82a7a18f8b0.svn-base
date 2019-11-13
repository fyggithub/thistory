#ifndef __stateReady_H__
#define __stateReady_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Grab.h"
#include "stateMoveWork.h"
#include "stateReset.h"
#include "stateMix.h"
#include "stateThrowCup.h"
#include "stateCup.h"
#include "SendResultCode.h"
#include "StepMotorGrab.h"
#define GRAB_X_YLINKAGEMOTOR1_SPEED      16     //试剂针X轴电机是从哪个速度台阶来运行的
#define GRAB_X_YLINKAGEMOTOR2_SPEED      16     //试剂针Y轴电机是从哪个速度台阶来运行的
#define ZAXIS_POSITION_SPEED   			 16     //Z轴到指定位置时速度
#define CATCHING_CUP_SPEED     			 16     //递杯时Y轴的速度
#define PUT_CUP_SPEED          			 16     //递杯时Y轴的速度
#define X_YLINGK_MIX_SPEED     			 16     //X，Y轴电机和混匀电机速度
#define X_YLINGK_MIX_SPEED2     		 16     //X，Y轴电机和混匀电机速度
#define X_YLINGK_MIX_SPEED3     		 16     //X，Y轴电机和混匀电机速度
#define ZAXIS_MIX_SPEED        			 16     //Z轴与混匀电机，Z轴的运动速度
#define THROW_CUP_SPEED1       			 16     //抛杯X轴速度
#define THROW_CUP_SPEED2       			 16     //抛杯Y轴速度

#define MOTOR_X_POSITIVE_DIRECTION 		0
#define MOTOR_Y_POSITIVE_DIRECTION 		1
#define MOTOR_Z_POSITIVE_DIRECTION 		0


VOID GrabStateEndHandler(VOID);
VOID GrabStateBasicCmdMessageHandler(Message* msg);
VOID GrabStateReadyMessageHandler(Message* msg);
INT32 DirSortOut(INT16 motor,INT16 step,INT16 dir);



#endif

