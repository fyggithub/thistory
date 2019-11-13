#include "HostRege.h"

/********************************************************************
 *查询软件版本
 *******************************************************************/
STATIC BOOL RegeHostCmdVersion(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 i = frame->data[0];
		msg.id = MSG_REGEHOST_VERSION;		
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
STATIC BOOL RegeHostCmdGetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *设置板卡信息
 *******************************************************************/
STATIC BOOL RegeHostCmdSetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *参数信息查询
 *******************************************************************/
STATIC BOOL RegeHostCmdParaInfo(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 8){
		msg.id = MSG_REGEHOST_PARINFO;
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
STATIC BOOL RegeHostCmdParaSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 8){
		msg.id = MSG_REGEHOST_PARSET;
		INT16 id1 = frame->data[0]*256 + frame->data[1];
		INT16 id2 = frame->data[2]*256 + frame->data[3];
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
STATIC BOOL RegeHostCmdWorkSave(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_REGEHOST_WRKSAVE;
		INT16 work = frame->data[0] * 256 + frame->data[1];
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
STATIC BOOL RegeHostCmdMotorEnable(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 motor_code  = frame->data[0];
		UINT8 enable_type = frame->data[1];
		msg.id = MSG_REGEHOST_MOTOR_ENABLE;
		msg.p1 = motor_code;
		msg.p2 = enable_type;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机复位
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorReset(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		msg.id = MSG_REGEHOST_MOTOR_RESET;
		msg.p1 = motor_code;
		msg.p2 = speed ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机回原点
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorToZero(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		msg.id = MSG_REGEHOST_MOTOR_TOZERO;
		msg.p1 = motor_code;
		msg.p2 = speed ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
 *电机相对运动
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorMovRel(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		UINT32 pluse = frame->data[2] * 256 + frame->data[3];
		UINT8 dir = frame->data[4];
		msg.id = MSG_REGEHOST_MOTOR_MOVREL;			
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
STATIC BOOL RegeHostCmdMotorMovAbs(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		UINT32 pluse = frame->data[2] * 256 + frame->data[3];
		msg.id = MSG_REGEHOST_MOTOR_MOVABS;
		msg.p1 = motor_code;
		msg.p2 = speed ;
		msg.p3 = pluse ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机运动到指定工作位置
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorMovWrk(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 3){
		UINT8 motor_code  = frame->data[0];
		UINT8 work = frame->data[1];
		UINT8 speed = frame->data[2];
		msg.id = MSG_REGEHOST_MOTOR_MOVWRK;
		msg.p1 = motor_code;
		msg.p2 = work ;
		msg.p3 = speed ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *Z轴检测液面
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorZaxisDet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 3){
		UINT8 work = frame->data[0];
		UINT8 speed = frame->data[1];
		UINT8 det_type = frame->data[2];
		msg.id = MSG_REGEHOST_MOTOR_ZAIXDET;
		msg.p1 = work;
		msg.p2 = speed ;
		msg.p3 = det_type ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *注射器吸排指定体积
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorSubDra(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 subdra = frame->data[0];
		UINT8 speed = frame->data[1];
		msg.id = MSG_REGEHOST_MOTOR_SUBDRA;
		msg.p1 = subdra;
		msg.p2 = speed ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *查询电机当前位置
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorInqurie(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){	
		msg.id = MSG_REGEHOST_MOTOR_INQUIRE;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *试剂针XY运动到工作位置
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorMovXY(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 work     = frame->data[0];
		UINT8 time_ctr = frame->data[1];
		msg.id = MSG_REGEHOST_MOTOR_MOVXY;
		msg.p1 = work;
		msg.p2 = time_ctr;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *试剂针下降-吸试剂-上升
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorZaxisSub(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 10){
		UINT16 work          = frame->data[0];
		UINT8 container_type = frame->data[1];
		UINT8 sub_type       = frame->data[2];
		UINT16 sub_cap       = frame->data[3] * 256 + frame->data[4];
		msg.id = MSG_REGEHOST_MOTOR_ZAXISSUB;
		msg.p1 = work;
		msg.p2 = container_type;
		msg.p3 = sub_type;
		msg.p4 = sub_cap;
		MessagePost(&msg);
		
		UINT8 down_type  = frame->data[5];
		INT32 margin     = frame->data[6] * 256 + frame->data[7];
		UINT8 time_ctr   = frame->data[8];
		UINT8 recmp_ctr  = frame->data[9];
		msg.id = MSG_REGEHOST_MOTOR_ZAXISSUB;
		msg.p1 = down_type;
		msg.p2 = margin;
		msg.p3 = time_ctr;
		msg.p4 = recmp_ctr;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *试剂针下降-排试剂-上升
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorZaxisDra(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 6){
		INT16 drain_cap   = frame->data[0] * 256 + frame->data[1];
		INT16 margin      = frame->data[2] * 256 + frame->data[3];
		UINT8 drain_type  = frame->data[4];
		UINT8 time_ctr    = frame->data[5];
		msg.id = MSG_REGEHOST_MOTOR_ZAXISDRA;
		msg.p1 = drain_cap;
		msg.p2 = margin;
		msg.p3 = drain_type;
		msg.p4 = time_ctr;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *试剂针下降-清洗-上升
 *******************************************************************/
STATIC BOOL RegeHostCmdMotorZaxisCle(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 9){
		UINT8 clean_type  = frame->data[0];
		UINT16 inner_time  = frame->data[1] * 256 + frame->data[2];
		UINT16 outer_time  = frame->data[3] * 256 + frame->data[4];
		UINT16 soap_time   = frame->data[5] * 256 + frame->data[6];
		UINT8 time_ctr    = frame->data[7];
		UINT8 recmp_ctr   = frame->data[8];
		msg.id = MSG_REGEHOST_MOTOR_ZAXISCLE;
		msg.p1 = clean_type;
		msg.p2 = inner_time;
		msg.p3 = outer_time;
		msg.p4 = soap_time;
		MessagePost(&msg);
		msg.id = MSG_REGEHOST_MOTOR_ZAXISCLE;
		msg.p1 = time_ctr;
		msg.p2 = recmp_ctr;
		msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *泵控制
 *******************************************************************/
STATIC BOOL RegeHostCmdPumpSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT16 pump_code   = frame->data[0] ;
		UINT8 tpye        = frame->data[1];
		UINT8 time        = frame->data[2] * 256 + frame->data[3];
		msg.id = MSG_REGEHOST_MOTOR_PUMPSET;
		msg.p1 = pump_code;
		msg.p2 = tpye;
		msg.p3 = time;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *泵查询
 *******************************************************************/
STATIC BOOL RegeHostCmdPumpGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 pump_code   = frame->data[0];
		msg.id = MSG_REGEHOST_MOTOR_PUMPGET;
		msg.p1 = pump_code;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 * 阀控制
 *******************************************************************/
STATIC BOOL RegeHostCmdValveSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT16 valve_code = frame->data[0];
		UINT8 tpye        = frame->data[1];
		UINT8 time        = frame->data[2] * 256 + frame->data[3];
		msg.id = MSG_REGEHOST_MOTOR_VALVESET;
		msg.p1 = valve_code;
		msg.p2 = tpye;
		msg.p3 = time;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *阀查询
 *******************************************************************/
STATIC BOOL RegeHostCmdValveGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 pump_code   = frame->data[0];
		msg.id = MSG_REGEHOST_MOTOR_VALVEGET;
		msg.p1 = pump_code;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *GPIO电平控制
 *******************************************************************/
STATIC BOOL RegeHostCmdIoSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT16 gpio_code   = frame->data[0];
		UINT8 type        = frame->data[1];
		msg.id = MSG_REGEHOST_MOTOR_IOSET;
		msg.p1 = gpio_code;
		msg.p2 = type;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *GPIO电平查询
 *******************************************************************/
STATIC BOOL RegeHostCmdIoGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 gpio_code   = frame->data[0];	
		msg.id = MSG_REGEHOST_MOTOR_IOGET;
		msg.p1 = gpio_code;	
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *查询光耦电平
 *******************************************************************/
STATIC BOOL RegeHostCmdOptoGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 code = frame->data[0];
		msg.id = MSG_REGEHOST_MOTOR_OPTOGET;
		msg.p1 = code;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *故障报警
 *******************************************************************/
STATIC BOOL RegeHostCmdFault(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT16 num = frame->data[0];
		UINT8 errorcode = frame->data[1];
		msg.id = MSG_REGEHOST_MOTOR_FAULT;
		msg.p1 = num;
		msg.p2 = errorcode;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *查询温度
 *******************************************************************/
STATIC BOOL RegeHostCmdTemp(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		msg.id = MSG_REGEHOST_LIQUID_WATCH;
		msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *延时指令
 *******************************************************************/
STATIC BOOL RegeHostCmdDelay(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
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
STATIC BOOL RegeHostStop(HostFrame* frame)
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
STATIC UINT32 HostRegeParser(HostFrame* frame)
{
	UINT8 tag = HostGetConsoleTag();
	BOOL ret = FALSE;
	if(!frame) return HOST_ACK_ERR_LEN;
	MainState.Parameter.receiveCmd = frame->command;
	switch(frame->command){
		case REGEHOST_CMD_VERSION       : ret = RegeHostCmdVersion (frame);      break;
		case REGEHOST_CMD_GETINFO       : ret = RegeHostCmdGetInfo (frame);      break;
		case REGEHOST_CMD_SETINFO       : ret = RegeHostCmdSetInfo (frame);      break;
		case REGEHOST_CMD_PARAINFO      : ret = RegeHostCmdParaInfo (frame);     break;
		case REGEHOST_CMD_PARASET       : ret = RegeHostCmdParaSet(frame);		 break;
		case REGEHOST_CMD_WRKSAVE       : ret = RegeHostCmdWorkSave(frame);		 break;
		case REGEHOST_CMD_MOTOR_ENABLE	: ret = RegeHostCmdMotorEnable (frame);  break;
		case REGEHOST_CMD_MOTOR_RESET	: ret = RegeHostCmdMotorReset  (frame);  break;
		case REGEHOST_CMD_MOTOR_TOZERO	: ret = RegeHostCmdMotorToZero (frame);  break;
		case REGEHOST_CMD_MOTOR_MOVREL	: ret = RegeHostCmdMotorMovRel (frame);  break;
		case REGEHOST_CMD_MOTOR_MOVABS	: ret = RegeHostCmdMotorMovAbs (frame);  break;
		case REGEHOST_CMD_MOTOR_MOVWRK	: ret = RegeHostCmdMotorMovWrk (frame);  break;
		case REGEHOST_CMD_MOTOR_ZAIXDET	: ret = RegeHostCmdMotorZaxisDet (frame);break;
		case REGEHOST_CMD_MOTOR_SUBDRA	: ret = RegeHostCmdMotorSubDra (frame);  break;
		case REGEHOST_CMD_MOTOR_INQUIRE	: ret = RegeHostCmdMotorInqurie (frame); break;
		case REGEHOST_CMD_MOTOR_MOVXY	: ret = RegeHostCmdMotorMovXY (frame);   break;
		case REGEHOST_CMD_MOTOR_ZAXISSUB: ret = RegeHostCmdMotorZaxisSub (frame);break;
		case REGEHOST_CMD_MOTOR_ZAXISDRA: ret = RegeHostCmdMotorZaxisDra (frame);break;
		case REGEHOST_CMD_MOTOR_ZAXISCLE: ret = RegeHostCmdMotorZaxisCle (frame);break;
		case REGEHOST_CMD_PUMPSET       : ret = RegeHostCmdPumpSet (frame);		 break;
		case REGEHOST_CMD_PUMPGET       : ret = RegeHostCmdPumpGet (frame);		 break;
		case REGEHOST_CMD_VALVESET      : ret = RegeHostCmdValveSet (frame);	 break;
		case REGEHOST_CMD_VALVEGET      : ret = RegeHostCmdValveGet (frame);	 break;
		case REGEHOST_CMD_IOSET			: ret = RegeHostCmdIoSet (frame);		 break;
		case REGEHOST_CMD_IOGET			: ret = RegeHostCmdIoGet (frame);		 break;
		case REGEHOST_CMD_OPTOGET		: ret = RegeHostCmdOptoGet (frame);		 break;
		case REGEHOST_CMD_FAULT			: ret = RegeHostCmdFault (frame);		 break;
		case REGEHOST_CMD_TEMP      	: ret = RegeHostCmdTemp(frame);			 break;	
		case REGEHOST_CMD_DEALY         : ret = RegeHostCmdDelay(frame);		 break;
		case REGEHOST_CMD_STOP			: ret = RegeHostStop(frame);			 break;
		default:
			DebugWarning (tag, "Found Unknown Command!");
			return (HOST_ACK_ERR_ADDR);;
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
VOID HostRegeInit(VOID)
{
	HostParserRegister (HOST_ADDR_REGE, HostRegeParser);
}
/********************************************************************
 * End of File
 *******************************************************************/






