#include "HostGrab.h"

/********************************************************************
 *查询软件版本
 *******************************************************************/
STATIC BOOL GrabHostCmdVersion(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 i = frame->data[0];
		msg.id = MSG_HOST_VERSION;		
		msg.p1 = i;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return TRUE;
}
/********************************************************************
 *查询板卡信息
 *******************************************************************/
STATIC BOOL GrabHostCmdGetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *设置板卡信息
 *******************************************************************/
STATIC BOOL GrabHostCmdSetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *参数信息查询
 *******************************************************************/
STATIC BOOL GrabHostCmdParaInfo(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 8){
		msg.id = MSG_HOST_PARINFO;
		INT16 id1 = frame->data[0]*256 + frame->data[1];
		INT16 id2 = frame->data[2]*256 + frame->data[3];
		INT16 id3 = frame->data[4]*256 + frame->data[5];
		INT16 id4 = frame->data[6]*256 + frame->data[7];
		msg.p1 = id1;
		msg.p2 = id2;
		msg.p3 = id3;
		msg.p4 = id4;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *参数信息设置
 *******************************************************************/
STATIC BOOL GrabHostCmdParaSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 8){
		msg.id = MSG_HOST_PARSET;
		UINT16 id1 = frame->data[0]*256 + frame->data[1];
		UINT16 id2 = frame->data[2]*256 + frame->data[3];
		INT16 hword1 = MAKE_WORD (frame->data[4], frame->data[5]);
		INT16 lword1 = MAKE_WORD (frame->data[6], frame->data[7]);
		INT32 value1 = MAKE_DWORD (hword1, lword1);
		INT16 hword2 = MAKE_WORD (frame->data[8], frame->data[9]);
		INT16 lword2 = MAKE_WORD (frame->data[10], frame->data[11]);
		INT32 value2 = MAKE_DWORD (hword2, lword2);
		msg.p1 = id1;
		msg.p2 = id2;
		msg.p3 = value1;
		msg.p4 = value2;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *工作位置保存
 *******************************************************************/
STATIC BOOL GrabHostCmdWorkSave(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_WRKSAVE;
		INT32 work = frame->data[0] * 256 + frame->data[1];
		msg.p1 = work;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机使能
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorEnable(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 motor_code  = frame->data[0];
		UINT8 enable_type = frame->data[1];
		msg.id = MSG_HOST_MOTOR_ENABLE;
		msg.p1 = motor_code;
		msg.p2 = enable_type;
		msg.p3 = msg.p4;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机复位
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorReset(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 motor_code = frame->data[0];
		UINT8 speed = frame->data[1];
		msg.id = MSG_HOST_MOTOR_RESET;
		msg.p1 = motor_code;
		msg.p2 = speed;
		msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机回原点
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorToZero(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		msg.id = MSG_HOST_MOTOR_TOZERO;
		msg.p1 = motor_code;
		msg.p2 = speed ;
		msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
 *电机相对运动
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorMovRel(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		UINT32 pluse = frame->data[2] * 256 + frame->data[3];
		UINT8 dir = frame->data[4];
		msg.id = MSG_HOST_MOTOR_MOVREL;			
		msg.p1 = motor_code;
		msg.p2 = speed ;
		msg.p3 = pluse ;
		msg.p4 = dir ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机运动到指定微步
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorMovAbs(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 3){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		UINT32 pluse = frame->data[2] * 256 + frame->data[3];
		msg.id = MSG_HOST_MOTOR_MOVABS;
		msg.p1 = motor_code;
		msg.p2 = speed ;
		msg.p3 = pluse ;
		msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机运动到指定工作位置
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorMovWrk(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 3){
		UINT8 motor_code  = frame->data[0];
		UINT8 work = frame->data[1];
		UINT8 speed = frame->data[2];
		msg.id = MSG_HOST_MOTOR_MOVWRK;
		msg.p1 = motor_code;
		msg.p2 = work ;
		msg.p3 = speed ;
		msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
 *查询电机当前位置
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorInqurie(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){	
		msg.id = MSG_HOST_MOTOR_INQUIRE;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *混匀电机控制
 *******************************************************************/
STATIC BOOL GrabHostCmdMixMotorCtrl(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){	
		UINT16 state = frame->data[0];
		UINT16 strength = frame->data[1];
		UINT16 type = frame->data[2];
		UINT32 mix_time  = frame->data[3]*256 + frame->data[4];		
		msg.id = MSG_HOST_MOTOR_MIXCTL;
		msg.p1 = state;
		msg.p2 = strength;
		msg.p3 = type;
		msg.p4 = mix_time;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *查询混匀电机
 *******************************************************************/
STATIC BOOL GrabHostCmdMixMotorInqurie(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){	
		msg.id = MSG_HOST_MOTOR_MIXINQUIRE;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *样本针XY运动到工作位置
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorMovXY(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT16 work  = frame->data[0];
		UINT16 way   = frame->data[1];
		UINT16 adjust = frame->data[2];	
		UINT8 time_ctr = frame->data[3];
		msg.id = MSG_HOST_MOTOR_MOVXY;
		msg.p1 = work;
		msg.p2 = way;
		msg.p3 = adjust;
		msg.p4 = time_ctr;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *抓杯手Z轴运动到当前的指定垂直位置
 *******************************************************************/
STATIC BOOL GrabHostCmdZaxisMotorVerticalWrk(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 3){
		UINT16 work  = frame->data[0];
		UINT16 vertical_work = frame->data[1];
		UINT8 time_ctr = frame->data[2];
		msg.id = MSG_HOST_MOTOR_ZAXISPOS;
		msg.p1 = work;
		msg.p2 = vertical_work;
		msg.p3 = time_ctr;
		msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *抓杯手递进抓杯
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorCatchCup(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 work  = frame->data[0];
		UINT16 time_ctl = frame->data[1];
		msg.id = MSG_HOST_MOTOR_CATCHCUP;
		msg.p1 = work;
		msg.p2 = time_ctl;
		msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *抓杯手后退放杯
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorPutCup(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 work  = frame->data[0];
		UINT16 time_ctl = frame->data[1];
		msg.id = MSG_HOST_MOTOR_PUTCUP;
		msg.p1 = work;
		msg.p2 = time_ctl;
		msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *抓手混匀X秒
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorMix(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT16 mix_strength = frame->data[0];
		UINT16 time = frame->data[1]*256 + frame->data[2];
		UINT16 type = frame->data[3];
		UINT8 time_ctl = frame->data[4];		
		msg.id = MSG_HOST_MOTOR_MIXTIME;
		msg.p1 = mix_strength;
		msg.p2 = time;
		msg.p3 = type;
		msg.p4 = time_ctl;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *抓手XY运动到工作位置并混匀X秒
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorXYMix(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 8){
		UINT16 work = frame->data[0];
		UINT16 hole = frame->data[1];
		UINT16 time_ctl1 = frame->data[2];
		UINT16 adjust = frame->data[3];		
		msg.id = MSG_HOST_MOTOR_MVMIX;
		msg.p1 = work;
		msg.p2 = hole;
		msg.p3 = time_ctl1;
		msg.p4 = adjust;
		MessagePost(&msg);		
		UINT16 mixstrength = frame->data[4];
		UINT16 mix_time = frame->data[5]*256 + frame->data[6];
		UINT16 type = frame->data[7];
		UINT16 timectl2 = frame->data[8];
		msg.id = MSG_HOST_MOTOR_MVMIX;
		msg.p1 = mixstrength;
		msg.p2 = mix_time;
		msg.p3 = type;
		msg.p4 = timectl2;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *抓手水平运动到抛杯位置
 *******************************************************************/
STATIC BOOL GrabHostCmdMotorThrowCup(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT16 work = frame->data[0];
		UINT16 code = frame->data[1];
		UINT16 time_ctl = frame->data[2];
		UINT16 type = frame->data[3];		
		msg.id = MSG_HOST_MOTOR_THROWCUP;
		msg.p1 = work;
		msg.p2 = code;
		msg.p3 = time_ctl;
		msg.p4 = type;
		MessagePost(&msg);		
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *泵控制
 *******************************************************************/
STATIC BOOL GrabHostCmdPumpSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 pump_code  = frame->data[0] ;
		UINT8 tpye        = frame->data[1];
		UINT8 time        = frame->data[2] * 256 + frame->data[3];
		msg.id = MSG_HOST_MOTOR_PUMPSET;
		msg.p1 = pump_code;
		msg.p2 = tpye;
		msg.p3 = time;
		msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *泵查询
 *******************************************************************/
STATIC BOOL GrabHostCmdPumpGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 pump_code   = frame->data[0];
		msg.id = MSG_HOST_MOTOR_PUMPGET;
		msg.p1 = pump_code;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 * 阀控制
 *******************************************************************/
STATIC BOOL GrabHostCmdValveSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 valve_code = frame->data[0] ;
		UINT8 tpye        = frame->data[1];
		UINT8 time        = frame->data[2] * 256 + frame->data[3];
		msg.id = MSG_HOST_MOTOR_VALVESET;
		msg.p1 = valve_code;
		msg.p2 = tpye;
		msg.p3 = time;
		msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *阀查询
 *******************************************************************/
STATIC BOOL GrabHostCmdValveGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 pump_code   = frame->data[0];
		msg.id = MSG_HOST_MOTOR_VALVEGET;
		msg.p1 = pump_code;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *GPIO电平控制
 *******************************************************************/
STATIC BOOL GrabHostCmdIoSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 gpio_code  = frame->data[0];
		UINT8 type        = frame->data[1];
		msg.id = MSG_HOST_MOTOR_IOSET;
		msg.p1 = gpio_code;
		msg.p2 = type;
		msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *GPIO电平查询
 *******************************************************************/
STATIC BOOL GrabHostCmdIoGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 gpio_code   = frame->data[0];	
		msg.id = MSG_HOST_MOTOR_IOGET;
		msg.p1 = gpio_code;
		msg.p2 = msg.p3 = msg.p4 = 0;		
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *查询光耦电平
 *******************************************************************/
STATIC BOOL GrabHostCmdOptoGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 code = frame->data[0];
		msg.id = MSG_HOST_MOTOR_OPTOGET;
		msg.p1 = code;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *故障报警
 *******************************************************************/
STATIC BOOL GrabHostCmdFault(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 num = frame->data[0];
		UINT8 errorcode = frame->data[1];
		msg.id = MSG_HOST_MOTOR_FAULT;
		msg.p1 = num;
		msg.p2 = errorcode;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *延时指令
 *******************************************************************/
STATIC BOOL GrabHostCmdDelay(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 delay = frame->data[0]*256 + frame->data[1];
		msg.id = MSG_DELAY;
		msg.p1 = delay;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *停机指令
 *******************************************************************/
STATIC BOOL GrabHostStop(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 i = frame->data[0];
		msg.id = MSG_STOP;		
		msg.p1 = i;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 HostGrabParser(HostFrame* frame)
{
	UINT8 tag = HostGetConsoleTag();
	BOOL ret = FALSE;
	if(!frame) return HOST_ACK_ERR_LEN;
	MainState.Parameter.receiveCmd = frame->command;
	switch(frame->command){
	case GRABHOST_CMD_VERSION       	 : ret = GrabHostCmdVersion (frame);     		 break;
	case GRABHOST_CMD_GETINFO       	 : ret = GrabHostCmdGetInfo (frame);     		 break;
	case GRABHOST_CMD_SETINFO       	 : ret = GrabHostCmdSetInfo (frame);    		 break;
	case GRABHOST_CMD_PARAINFO      	 : ret = GrabHostCmdParaInfo (frame);    		 break;
	case GRABHOST_CMD_PARASET			 : ret = GrabHostCmdParaSet(frame);				 break;
	case GRABHOST_CMD_WRKSAVE            : ret = GrabHostCmdWorkSave(frame);             break;
	case GRABHOST_CMD_MOTOR_ENABLE	 	 : ret = GrabHostCmdMotorEnable (frame); 		 break;
	case GRABHOST_CMD_MOTOR_RESET		 : ret = GrabHostCmdMotorReset  (frame); 		 break;
	case GRABHOST_CMD_MOTOR_TOZERO	 	 : ret = GrabHostCmdMotorToZero (frame); 		 break;
	case GRABHOST_CMD_MOTOR_MOVREL	 	 : ret = GrabHostCmdMotorMovRel (frame); 		 break;
	case GRABHOST_CMD_MOTOR_MOVABS	 	 : ret = GrabHostCmdMotorMovAbs (frame);	 	 break;
	case GRABHOST_CMD_MOTOR_MOVWRK	 	 : ret = GrabHostCmdMotorMovWrk (frame); 		 break;
	case GRABHOST_CMD_MOTOR_INQUIRE      : ret = GrabHostCmdMotorInqurie(frame);         break;
	case GRABHOST_CMD_MIXMOTOR_CTRL  	 : ret = GrabHostCmdMixMotorCtrl(frame);		 break;
	case GRABHOST_CMD_MIXMOTOR_INQUIRE   : ret = GrabHostCmdMixMotorInqurie(frame);		 break;
	case GRABHOST_CMD_MOTOR_MOVXY        : ret = GrabHostCmdMotorMovXY(frame);			 break;
	case GRABHOST_CMD_ZAXISMOTOR_MOVWEK  : ret = GrabHostCmdZaxisMotorVerticalWrk(frame);break;
	case GRABHOST_CMD_CATCH_CUP          : ret = GrabHostCmdMotorCatchCup(frame);		 break;
	case GRABHOST_CMD_PUT_CUP        	 : ret = GrabHostCmdMotorPutCup(frame);        	 break;
	case GRABHOST_CMD_MIX                : ret = GrabHostCmdMotorMix(frame);			 break;
	case GRABHOST_CMD_XY_MIX         	 : ret = GrabHostCmdMotorXYMix(frame);			 break;
	case GRABHOST_CMD_THROW_CUP			 : ret = GrabHostCmdMotorThrowCup(frame);		 break;
	case GRABHOST_CMD_PUMPSET       	 : ret = GrabHostCmdPumpSet (frame);			 break;
	case GRABHOST_CMD_PUMPGET       	 : ret = GrabHostCmdPumpGet (frame);			 break;
	case GRABHOST_CMD_VALVESET      	 : ret = GrabHostCmdValveSet (frame);			 break;
	case GRABHOST_CMD_VALVEGET      	 : ret = GrabHostCmdValveGet (frame);			 break;
	case GRABHOST_CMD_IOSET			 	 : ret = GrabHostCmdIoSet (frame);				 break;
	case GRABHOST_CMD_IOGET			 	 : ret = GrabHostCmdIoGet (frame);				 break;
	case GRABHOST_CMD_OPTOGET			 : ret = GrabHostCmdOptoGet (frame);			 break;
	case GRABHOST_CMD_FAULT			 	 : ret = GrabHostCmdFault (frame);				 break;
	case GRABHOST_CMD_DEALY          	 : ret = GrabHostCmdDelay(frame);				 break;
	case GRABHOST_CMD_STOP				 : ret = GrabHostStop(frame);					 break;
	default:
		DebugWarning (tag, "Found Unknown Command!");
		return (HOST_ACK_ERR_ADDR);
	}
	if(ret){
		return (HOST_ACK_SUCCESS);
	}else{
		DebugWarning(tag, "Found Length Error!");
		return (HOST_ACK_ERR_LEN);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID HostGrabInit(VOID)
{
	HostParserRegister (HOST_ADDR_GRAB, HostGrabParser);
}
/********************************************************************
 * End of File
 *******************************************************************/




