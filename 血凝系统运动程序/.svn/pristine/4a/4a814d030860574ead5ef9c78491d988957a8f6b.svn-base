#ifndef __stateClean_H__
#define __stateClean_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Regent.h"
#include "SendResultCode.h"
#include "math.h"
#include "SlaveRege.h"
#include "ConfigReage.h"
#include "StepMotorRege.h"

#define K1    1     		//清洗开环预热补偿系数
#define Ta    28    		//室温
//#define t1    2000  		//开环预热时间 
#define K2    1.5    		    //试剂开环预热补偿系数
//#define t2    2000		    //试剂开环预热时间

#define CLN_HEAT_TIME  1000     //清洗预热时间
#define CLN_STOP_TIME  0     //清洗停止时间
#define REGE_HEAT_TIME  3000     //试剂预热时间
#define REGE_STOP_TIME  0     //试剂停止时间

#define OPEN_SUB_HEAT  1      //试剂开启开环PID  1 - 开启   0 - 不开启
#define OPEN_CLN_HEAT  0      //清洗开启开环PID  1 - 开启   0 - 不开启

#define OPEN_HEAT_TIME 1700

enum {
	STATE1_CLN_BASIC = 0, STATE1_CLN_DOWN, STATE1_CLN_CLN, \
	STATE1_CLN_UP, 		  STATE1_CLN_MAX, 
};

enum {
	STATE2_CLN_CLN_DRA = 0,STATE2_CLN_CLN_SUB,\
	STATE2_CLN_CLN_WAIT,   STATE2_CLN_CLN_MAX,
};

INT16 GetClnPwm(VOID);
INT16 GetSubPwm(VOID);
VOID RegeStateClnMessageHandler(Message* msg);

#endif


