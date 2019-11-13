#ifndef __stateMoveWork_H__
#define __stateMoveWork_H__

#include "Board.h"
#include "Console.h"
#include "StepMotor.h"
#include "Datatypes.h"
#include "Message.h"
#include "Regent.h"
#include "SendResultCode.h"
#include "ConfigReage.h"
#include "StepMotorRege.h"

#define REAGENT_BIGHOLE_CODE            1      //针水平位置1起始参数编号
#define REAGENT_BIGHOLE_NUM             2      //针水平位置1的参数个数
#define REAGENT_MIDHOLE_CODE            21     //针水平位置中孔区起始参数编号
#define REAGENT_MIDHOLE_NUM             12     //针水平位置中孔区的参数个数
#define REAGENT_SMALLHOLE_CODE          51     //针水平位置小孔区起始参数编号
#define REAGENT_SMALLHOLE_NUM           14     //针水平位置小孔区的参数个数
#define REAGENT_LEVEL_CODE        		101    //针水平位置2起始参数编号
#define REAGENT_LEVEL_NUM               6      //针水平位置2的参数个数

enum{STATE1_MV_BASIC = 0, STATE1_MV_MV, STATE1_MV_MAX};
	
VOID RegeStateMvMessageHandler(Message* msg);
VOID RegeWrkStateMessageHandler(INT16 motor,INT32 work_position, UINT32 speed);

#endif



