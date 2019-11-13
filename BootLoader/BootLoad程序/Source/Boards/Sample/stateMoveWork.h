#ifndef __stateMoveWork_H__
#define __stateMoveWork_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "SendResultCode.h"
#include "Sample.h"
#include "ConfigSample.h"
#include "StepMotorSample.h"

#define SAMPLE_RACK_STARTCODE                       1      //��ˮƽλ����������ʼ�������
#define SAMPLE_RACK_NUM                             80     //��ˮƽλ�������ܵĲ�������
#define SAMPLE_COOLBIT_STARTCODE                	151    //��ˮƽλ������Լ�λ��ʼ�������
#define SAMPLE_COOLBIT_NUM                       	7      //��ˮƽλ������Լ�λ��������
#define SAMPLE_ROOMTEMPERATURE_STARTCODE            181    //��ˮƽλ�ó����Լ�λ��ʼ�������
#define SAMPLE_ROOMTEMPERATURE_NUM                  4      //��ˮƽλ�ó����Լ�λ��������
#define SAMPLE_NEEDLESHORIZONTAL_STARTCODE          201    //��ˮƽλ��2��ʼ�������
#define SAMPLE_NEEDLESHORIZONTAL_NUM                7      //��ˮƽλ��2�Ĳ�������

enum {
	STATE1_MV_BASIC = 0, STATE1_MV_MV, STATE1_MV_MAX,
};

VOID SampleWrkStateMessageHandler(INT16 motor,INT32 work_position, UINT32 speed);
VOID SampleStateMvMessageHandler(Message* msg);


#endif


