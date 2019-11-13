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


#define REAGENT_SUBDAR_SPEED        		 15     //ע��������Һ���ٶ�
#define REAGENT_X_YLINKAGEMOTOR1_SPEED       15     //������X�����Ǵ��ĸ��ٶ�̨�������е�
#define REAGENT_X_YLINKAGEMOTOR2_SPEED       15     //������Y�����Ǵ��ĸ��ٶ�̨�������е�
#define REAGENT_ZAXISSUBSAM_RISESPEED        15     //�������½�-���Լ�-����  Z�������ٶ� 
#define REAGENT_ZAXISSUBSAM_FALLSPEED        15      //�������½�-���Լ�-����  Z���½��ٶ� 
#define REAGENT_INJSUBSAM_SPEED              15     //�������½�-���Լ�-����  ��Һ���ٶ�
#define REAGENT_ZAXISDRAIN_RISESPEED         15     //�������½�-����-���� Z�������ٶ�
#define REAGENT_ZAXISDRAIN_FALLSPEED         15      //�������½�-����-���� Z���½��ٶ�
#define REAGENT_INJDRAIN_SPEED               15     //�������½�-����-���� ��Һ���ٶ�
#define REAGENT_ZAXISDRAINCLE_RISESPEED      15     //�������½�-��ϴ-����  Z�������ٶ�
#define REAGENT_ZAXISDRAINCLE_FALLSPEED      15      //�������½�-��ϴ-����  Z���½��ٶ�
#define REAGENT_INJDRAINCLE_SPEED            15     //�������½�-��ϴ-����  ����Һ���ٶ�

//1 - ���������Զ�����  0 - ���������Զ�����
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

