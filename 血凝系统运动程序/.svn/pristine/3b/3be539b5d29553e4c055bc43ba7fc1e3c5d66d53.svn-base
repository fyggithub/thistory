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

#define SAMPLE_RACK_STARTCODE                       1      //针水平位置样本架起始参数编号
#define SAMPLE_RACK_NUM                             80     //针水平位置样本架的参数个数
#define SAMPLE_COOLBIT_STARTCODE                	151    //针水平位置冷藏试剂位起始参数编号
#define SAMPLE_COOLBIT_NUM                       	7      //针水平位置冷藏试剂位参数个数
#define SAMPLE_ROOMTEMPERATURE_STARTCODE            181    //针水平位置常温试剂位起始参数编号
#define SAMPLE_ROOMTEMPERATURE_NUM                  4      //针水平位置常温试剂位参数个数
#define SAMPLE_NEEDLESHORIZONTAL_STARTCODE          201    //针水平位置2起始参数编号
#define SAMPLE_NEEDLESHORIZONTAL_NUM                7      //针水平位置2的参数个数

enum {
	STATE1_MV_BASIC = 0, STATE1_MV_MV, STATE1_MV_MAX,
};

VOID SampleWrkStateMessageHandler(INT16 motor,INT32 work_position, UINT32 speed);
VOID SampleStateMvMessageHandler(Message* msg);


#endif


