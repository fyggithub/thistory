#include "SendResultCode.h"

/********************************************************************
 *发送查询参数的值
 *******************************************************************/
STATIC VOID SendParameterMessage(VOID)
{
	INT32 value1,value2,value3,value4;
	BYTE sendBuf[25];	
	INT16 id1 = MainState.Parameter.codeID1;
	INT16 id2 = MainState.Parameter.codeID2;
	INT16 id3 = MainState.Parameter.codeID3;
	INT16 id4 = MainState.Parameter.codeID4;
	
	if(id1 >= CFG_REAGENT_POSLMT_VNEG)
		value1 = ConfigGetDword(id1);
	else
		value1 = 0;
	if(id2 >= CFG_REAGENT_POSLMT_VNEG)
		value2 = ConfigGetDword(id2);
	else
		value2 = 0;
	if(id3 >= CFG_REAGENT_POSLMT_VNEG)
		value3 = ConfigGetDword(id3);
	else
		value3 = 0;
	if(id4 >= CFG_REAGENT_POSLMT_VNEG)
		value4 = ConfigGetDword(id4);
	else
		value4 = 0;
	sendBuf[0] = 0;   //状态单元ok
	sendBuf[1] = HBYTE (id1);
	sendBuf[2] = LBYTE (id1);
	sendBuf[3] = HBYTE (id2);
	sendBuf[4] = LBYTE (id2);
	sendBuf[5] = HBYTE (id3);
	sendBuf[6] = LBYTE (id3);
	sendBuf[7] = HBYTE (id4);
	sendBuf[8] = LBYTE (id4);
	sendBuf[9]  = HBYTE (HWORD(value1));
	sendBuf[10] = HBYTE (LWORD(value1));
	sendBuf[11] = LBYTE (HWORD(value1));
	sendBuf[12] = LBYTE (LWORD(value1));
	sendBuf[13] = HBYTE (HWORD(value2));
	sendBuf[14] = HBYTE (LWORD(value2));
	sendBuf[15] = LBYTE (HWORD(value2));
	sendBuf[16] = LBYTE (LWORD(value2));
	sendBuf[17] = HBYTE (HWORD(value3));
	sendBuf[18] = HBYTE (LWORD(value3));
	sendBuf[19] = LBYTE (HWORD(value3));
	sendBuf[20] = LBYTE (LWORD(value3));
	sendBuf[21] = HBYTE (HWORD(value4));
	sendBuf[22] = HBYTE (LWORD(value4));
	sendBuf[23] = LBYTE (HWORD(value4));
	sendBuf[24] = LBYTE (LWORD(value4));
	HostResponse(sendBuf, 25);
}
/********************************************************************
 *参数设置后的结果码
 *******************************************************************/
STATIC VOID SendSetParameterMessage(VOID)
{
	BYTE sendBuf[1];
	sendBuf[0] = 0;
	HostResponse(sendBuf, 1);
}
/********************************************************************
 *停机结果码
 *******************************************************************/
STATIC VOID SendStopMessage(VOID)
{
	BYTE sendBuf[1];
	sendBuf[0] = 0;
	HostResponse(sendBuf, 1);
}
/********************************************************************
 *工作位置保存的结果码
 *******************************************************************/
STATIC VOID SendWorkSaveMessage(VOID)
{
	BYTE sendBuf[1];
	sendBuf[0] = 0;
	HostResponse(sendBuf, 1);
}
/********************************************************************
 *查询电机当前位置
 *******************************************************************/
STATIC VOID SendMotorPos(VOID)
{
	INT32 xpluse,ypluse,zpluse,synpluse;
	BYTE sendBuf[20];
	xpluse = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
	ypluse = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
	zpluse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
	synpluse = StepMotorGetPos (&(MainState.motors[MOTOR_SYN]));
	sendBuf[0] = 0;  //状态单元
	sendBuf[1] = HBYTE (LWORD(xpluse));
	sendBuf[2] = LBYTE (HWORD(xpluse));
	sendBuf[3] = LBYTE (LWORD(xpluse));
	sendBuf[4] = HBYTE (LWORD(ypluse));
	sendBuf[5] = LBYTE (HWORD(ypluse));
	sendBuf[6] = LBYTE (LWORD(ypluse));
	sendBuf[7] = HBYTE (LWORD(zpluse));
	sendBuf[8] = LBYTE (HWORD(zpluse));
	sendBuf[9] = LBYTE (LWORD(zpluse));
	sendBuf[10] = HBYTE (LWORD(synpluse));
	sendBuf[11] = LBYTE (HWORD(synpluse));
	sendBuf[12] = LBYTE (LWORD(synpluse));
	HostResponse(sendBuf, 13);
}
/********************************************************************
 *发送结果码
 *******************************************************************/
STATIC VOID SendCombinationMessage(VOID)
{
	INT32 xpluse,ypluse,zpluse,synpluse;
	BYTE respBuf[23];
	xpluse = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
	ypluse = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
	zpluse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
	synpluse = StepMotorGetPos (&(MainState.motors[MOTOR_SYN]));
	respBuf[0] = 0;
	respBuf[1] = 0;
	respBuf[2] = MainState.Parameter.wrkCode;
	respBuf[3] = HBYTE (LWORD(xpluse));
	respBuf[4] = LBYTE (HWORD(xpluse));
	respBuf[5] = LBYTE (LWORD(xpluse));
	respBuf[6] = HBYTE (LWORD(ypluse));
	respBuf[7] = LBYTE (HWORD(ypluse));
	respBuf[8] = LBYTE (LWORD(ypluse));
	respBuf[9] = 0;
	respBuf[10] = HBYTE (LWORD(zpluse));
	respBuf[11] = LBYTE (HWORD(zpluse));
	respBuf[12] = LBYTE (LWORD(zpluse));
	respBuf[13] = 0;
	respBuf[14] = HBYTE (LWORD(synpluse));
	respBuf[15] = LBYTE (HWORD(synpluse));
	respBuf[16] = LBYTE (LWORD(synpluse));
	respBuf[17] = 0;
	respBuf[18] = 0;
	respBuf[19] = 0;	
	respBuf[20] = 0;
	respBuf[21] = 0;
	respBuf[22] = 0x25;	
//	respBuf[0] = 0;
//	respBuf[1] = 0;
//	respBuf[2] = MainState.Parameter.wrkCode;
//	respBuf[3] = LBYTE (HWORD(xpluse));
//	respBuf[4] = HBYTE (LWORD(xpluse));
//	respBuf[5] = LBYTE (LWORD(xpluse));
//	respBuf[6] = LBYTE (HWORD(ypluse));
//	respBuf[7] = HBYTE (LWORD(ypluse));
//	respBuf[8] = LBYTE (LWORD(ypluse));
//	respBuf[9] = 0;
//	respBuf[10] = LBYTE (HWORD(zpluse));
//	respBuf[11] = HBYTE (LWORD(zpluse));
//	respBuf[12] = LBYTE (LWORD(zpluse));
//	respBuf[13] = 0;
//	respBuf[14] = LBYTE (HWORD(synpluse));
//	respBuf[15] = HBYTE (LWORD(synpluse));
//	respBuf[16] = LBYTE (LWORD(synpluse));
//	respBuf[17] = 0;
//	respBuf[18] = 0;
//	respBuf[19] = 0;	
//	respBuf[20] = 0;
//	respBuf[21] = 0;
//	respBuf[22] = 0x25;
	HostResponse(respBuf, 23);
}
/********************************************************************
 *发送版本号
 *******************************************************************/
STATIC VOID SendVersionMessage(VOID)
{
	BYTE code1,code2,code3;
	BYTE respBuf[4];
	code1 = VERSION_XX;
	code2 = VERSION_YY;
	code3 = VERSION_ZZ;
	respBuf[0] = 0;
	respBuf[1] = code1;	
	respBuf[2] = code2;	
	respBuf[3] = code3;
	HostResponse(respBuf, 4);
}
/********************************************************************
 *发送吸试剂Z轴下降最低端高度
 *******************************************************************/
STATIC VOID SendSubMessage(VOID)
{
	INT32 xpluse,ypluse,zpluse,synpluse;
	BYTE respBuf[23];
	xpluse = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
	ypluse = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
	zpluse = MainState.Parameter.zLowestPulse;
	synpluse = StepMotorGetPos (&(MainState.motors[MOTOR_SYN]));
	respBuf[0] = 0;
	respBuf[1] = 0;
	respBuf[2] = MainState.Parameter.wrkCode;
	respBuf[3] = HBYTE (LWORD(xpluse));
	respBuf[4] = LBYTE (HWORD(xpluse));
	respBuf[5] = LBYTE (LWORD(xpluse));
	respBuf[6] = HBYTE (LWORD(ypluse));
	respBuf[7] = LBYTE (HWORD(ypluse));
	respBuf[8] = LBYTE (LWORD(ypluse));
	respBuf[9] = 0;
	respBuf[10] = HBYTE (LWORD(zpluse));
	respBuf[11] = LBYTE (HWORD(zpluse));
	respBuf[12] = LBYTE (LWORD(zpluse));
	respBuf[13] = 0;
	respBuf[14] = HBYTE (LWORD(synpluse));
	respBuf[15] = LBYTE (HWORD(synpluse));
	respBuf[16] = LBYTE (LWORD(synpluse));
	respBuf[17] = 0;
	respBuf[18] = 0;
	respBuf[19] = 0;	
	respBuf[20] = 0;
	respBuf[21] = 0;
	respBuf[22] = 0x25;	
//	respBuf[0] = 0;
//	respBuf[1] = 0;
//	respBuf[2] = MainState.Parameter.wrkCode;
//	respBuf[3] = LBYTE (HWORD(xpluse));
//	respBuf[4] = HBYTE (LWORD(xpluse));
//	respBuf[5] = LBYTE (LWORD(xpluse));
//	respBuf[6] = LBYTE (HWORD(ypluse));
//	respBuf[7] = HBYTE (LWORD(ypluse));
//	respBuf[8] = LBYTE (LWORD(ypluse));
//	respBuf[9] = 0;
//	respBuf[10] = LBYTE (HWORD(zpluse));
//	respBuf[11] = HBYTE (LWORD(zpluse));
//	respBuf[12] = LBYTE (LWORD(zpluse));
//	respBuf[13] = 0;
//	respBuf[14] = LBYTE (HWORD(synpluse));
//	respBuf[15] = HBYTE (LWORD(synpluse));
//	respBuf[16] = LBYTE (LWORD(synpluse));
//	respBuf[17] = 0;
//	respBuf[18] = 0;
//	respBuf[19] = 0;	
//	respBuf[20] = 0;
//	respBuf[21] = 0;
//	respBuf[22] = 0x25;
	HostResponse(respBuf, 23);
}
/********************************************************************
 *结果码返回函数
 *******************************************************************/
VOID SendResultMessage(VOID)
{ 
	MainState.stateManageTop = STATE0_READY;
	switch(MainState.Parameter.receiveCmd)
	{
		case REGEHOST_CMD_VERSION       : SendVersionMessage();			break;
		case REGEHOST_CMD_GETINFO       :    break;
		case REGEHOST_CMD_SETINFO       :    break;
		case REGEHOST_CMD_PARAINFO      : SendParameterMessage();       break;   
		case REGEHOST_CMD_PARASET       : SendSetParameterMessage(); 	break;
		case REGEHOST_CMD_WRKSAVE       : SendWorkSaveMessage();	    break;
		case REGEHOST_CMD_MOTOR_INQUIRE	: SendMotorPos();               break;   	//发送电机的坐标位置   
		case REGEHOST_CMD_DEALY         :
		case REGEHOST_CMD_MOTOR_ENABLE	:  
		case REGEHOST_CMD_MOTOR_RESET	:
		case REGEHOST_CMD_MOTOR_TOZERO	: 
		case REGEHOST_CMD_MOTOR_MOVREL	: 
		case REGEHOST_CMD_MOTOR_MOVABS	: 
		case REGEHOST_CMD_MOTOR_MOVWRK	:
		case REGEHOST_CMD_MOTOR_ZAIXDET	:
		case REGEHOST_CMD_MOTOR_SUBDRA	:    		
		case REGEHOST_CMD_MOTOR_MOVXY	: 		
		case REGEHOST_CMD_MOTOR_ZAXISDRA: 
		case REGEHOST_CMD_MOTOR_ZAXISCLE: SendCombinationMessage(); break;
		case REGEHOST_CMD_MOTOR_ZAXISSUB: SendSubMessage();	break;
		case REGEHOST_CMD_PUMPSET       : 	break;
		case REGEHOST_CMD_PUMPGET       : 	break;
		case REGEHOST_CMD_VALVESET      : 	break;
		case REGEHOST_CMD_VALVEGET      : 	break;
		case REGEHOST_CMD_IOSET			: 	break;
		case REGEHOST_CMD_IOGET			: 	break;
		case REGEHOST_CMD_OPTOGET		: 	break;
		case REGEHOST_CMD_FAULT			:	break;
		case REGEHOST_CMD_STOP			:	SendStopMessage();			break;
		default:break;
	}
}





