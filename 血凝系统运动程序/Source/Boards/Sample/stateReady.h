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

#define SAMPLE_SUBDAR_SPEED        		 	15     //ע��������Һ���ٶ�
#define SAMPLE_X_YLINKAGEMOTOR1_SPEED       15     //������X�����Ǵ��ĸ��ٶ�̨�������е�
#define SAMPLE_X_YLINKAGEMOTOR2_SPEED       15     //������Y�����Ǵ��ĸ��ٶ�̨�������е�
#define SAMPLE_ZAXISSUBSAM_RISESPEED        15     //�������½�-���Լ�-����  Z�������ٶ� 
#define SAMPLE_ZAXISSUBSAM_FALLSPEED        15     //�������½�-���Լ�-����  Z���½��ٶ� 
#define SAMPLE_INJSUBSAM_SPEED              15     //�������½�-���Լ�-����  ��Һ���ٶ�
#define SAMPLE_ZAXISDRAIN_RISESPEED         15     //�������½�-����-���� Z�������ٶ�
#define SAMPLE_ZAXISDRAIN_FALLSPEED         15     //�������½�-����-���� Z���½��ٶ�
#define SAMPLE_INJDRAIN_SPEED               15      //�������½�-����-���� ��Һ���ٶ�
#define SAMPLE_ZAXISDRAINCLE_RISESPEED      15     //�������½�-��ϴ-����  Z�������ٶ�
#define SAMPLE_ZAXISDRAINCLE_FALLSPEED      15     //�������½�-��ϴ-����  Z���½��ٶ�
#define SAMPLE_INJDRAINCLE_SPEED            15      //�������½�-��ϴ-����  ����Һ���ٶ�

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



