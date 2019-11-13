#ifndef __stateMoveWork_H__
#define __stateMoveWork_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Timer.h"
#include "Grab.h"
#include "SendResultCode.h" 
#include "stateReady.h"
#include "ConfigGrab.h"
#include "StepMotorGrab.h"
//#define POSTION_ADDPLUSE     148    //在初始位置在加上250个脉冲，主要是Z轴抬杯位置

#define GRAB_MEASUREMENTBIT_STARTCODE               1      //水平位置光学测量位置起始编码
#define GRAB_MEASUREMENTBIT_NUM                     12     //水平位置光学测量位置参数个数
#define GRAB_INCUBATIONBIT_STARTCODE                21     //水平位置孵育位起始编码
#define GRAB_INCUBATIONBIT_NUM                      18     //水平位置孵育参数个数
#define GRAB_MAGNETICBEADCUP_STARTCODE              61     //水平位置磁珠手动放杯起始编号        
#define GRAB_MAGNETICBEADCUP_NUM                    12      //水平位置磁珠手动放杯参数个数
#define GRAB_MAGNETICBEAD_MEASURE_STARTCODE         51     //水平位置磁珠法测量位置起始编码
#define GRAB_MAGNETICBEAD_MEASURE_NUM               1      //水平位置磁珠法测量位置参数个数
#define GRAB_COOPERATION_STARTCODE                  101    //水平位置抓杯手配合起始编号
#define GRAB_COOPERATION_NUM                        3      //水平位置抓杯手配合参数个数
#define GRAB_THROWCUP_STARTCODE                     111    //水平位置抛杯位置起始编号
#define GRAB_THROWCUP_NUM                           2      //水平位置抛杯位置参数个数
#define GRAB_EXTREMEPOSITION_STARTCODE              204    //水平位置极限位置起始编号
#define GRAB_EXTREMEPOSITION_NUM                    4      //水平位置极限位置参数个数

enum {
	STATE1_MV_BASIC = 0, STATE1_MV_MV, STATE1_MV_MAX,
};

enum{
	STATE1_ZAXIS_BASIC = 0,STATE1_ZAXIS_MV,STATE1_ZAXIS_MAX
};

VOID GrabWrkStateMessageHandler(INT16 motor,INT32 work_position, UINT32 speed,UINT8 type);
VOID GrabStateMvMessageHandler(Message* msg);
VOID ZAxisSpecifyVerticalPos(Message* msg);

#endif


