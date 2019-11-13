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
#define GRAB_X_YLINKAGEMOTOR1_SPEED      16     //�Լ���X�����Ǵ��ĸ��ٶ�̨�������е�
#define GRAB_X_YLINKAGEMOTOR2_SPEED      16     //�Լ���Y�����Ǵ��ĸ��ٶ�̨�������е�
#define ZAXIS_POSITION_SPEED   			 16     //Z�ᵽָ��λ��ʱ�ٶ�
#define CATCHING_CUP_SPEED     			 16     //�ݱ�ʱY����ٶ�
#define PUT_CUP_SPEED          			 16     //�ݱ�ʱY����ٶ�
#define X_YLINGK_MIX_SPEED     			 16     //X��Y�����ͻ��ȵ���ٶ�
#define X_YLINGK_MIX_SPEED2     		 16     //X��Y�����ͻ��ȵ���ٶ�
#define X_YLINGK_MIX_SPEED3     		 16     //X��Y�����ͻ��ȵ���ٶ�
#define ZAXIS_MIX_SPEED        			 16     //Z������ȵ����Z����˶��ٶ�
#define THROW_CUP_SPEED1       			 16     //�ױ�X���ٶ�
#define THROW_CUP_SPEED2       			 16     //�ױ�Y���ٶ�

#define MOTOR_X_POSITIVE_DIRECTION 		0
#define MOTOR_Y_POSITIVE_DIRECTION 		1
#define MOTOR_Z_POSITIVE_DIRECTION 		0


VOID GrabStateEndHandler(VOID);
VOID GrabStateBasicCmdMessageHandler(Message* msg);
VOID GrabStateReadyMessageHandler(Message* msg);
INT32 DirSortOut(INT16 motor,INT16 step,INT16 dir);



#endif

