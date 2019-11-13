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
	
	if(id1 >= CFG_SAMPLE_POSLMT_VNEG)
		value1 = ConfigGetDword(id1);
	else
		value1 = 0;
	if(id2 >= CFG_SAMPLE_POSLMT_VNEG)
		value2 = ConfigGetDword(id2);
	else
		value2 = 0;
	if(id3 >= CFG_SAMPLE_POSLMT_VNEG)
		value3 = ConfigGetDword(id3);
	else
		value3 = 0;
	if(id4 >= CFG_SAMPLE_POSLMT_VNEG)
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
	sendBuf[10] = LBYTE (HWORD(value1));
	sendBuf[11] = HBYTE (LWORD(value1));
	sendBuf[12] = LBYTE (LWORD(value1));
	sendBuf[13] = HBYTE (HWORD(value2));
	sendBuf[14] = LBYTE (HWORD(value2));
	sendBuf[15] = HBYTE (LWORD(value2));
	sendBuf[16] = LBYTE (LWORD(value2));
	sendBuf[17] = HBYTE (HWORD(value3));
	sendBuf[18] = LBYTE (HWORD(value3));
	sendBuf[19] = HBYTE (LWORD(value3));
	sendBuf[20] = LBYTE (LWORD(value3));
	sendBuf[21] = HBYTE (HWORD(value4));
	sendBuf[22] = LBYTE (HWORD(value4));
	sendBuf[23] = HBYTE (LWORD(value4));
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
	BYTE sendBuf[14];
	xpluse = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
	ypluse = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
	zpluse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
	synpluse = StepMotorGetPos (&(MainState.motors[MOTOR_SYN]));
	sendBuf[0] = 0;  //状态单元
	sendBuf[1] = 0;
	sendBuf[2] = LBYTE (HWORD(xpluse));
	sendBuf[3] = HBYTE (LWORD(xpluse));
	sendBuf[4] = LBYTE (LWORD(xpluse));
	sendBuf[5] = LBYTE (HWORD(ypluse));
	sendBuf[6] = HBYTE (LWORD(ypluse));
	sendBuf[7] = LBYTE (LWORD(ypluse));
	sendBuf[8] = LBYTE (HWORD(zpluse));
	sendBuf[9] = HBYTE (LWORD(zpluse));
	sendBuf[10] = LBYTE (LWORD(zpluse));
	sendBuf[11] = LBYTE (HWORD(synpluse));
	sendBuf[12] = HBYTE (LWORD(synpluse));
	sendBuf[13] = LBYTE (LWORD(synpluse));
	HostResponse(sendBuf, 14);
}

/********************************************************************
 *发送结果码
 *******************************************************************/
STATIC VOID SendCombinationMessage(VOID)
{
	INT32 xpluse,ypluse,zpluse,synpluse;
	BYTE respBuf[21];
	xpluse = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
	ypluse = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
	zpluse = StepMotorGetPos (&(MainState.motors[MOTOR_Z]));
	synpluse = StepMotorGetPos (&(MainState.motors[MOTOR_SYN]));
	respBuf[0] = 0;
	respBuf[1] = 0;
	respBuf[2] = MainState.Parameter.wrkCode;
	respBuf[3] = LBYTE (HWORD(xpluse));
	respBuf[4] = HBYTE (LWORD(xpluse));
	respBuf[5] = LBYTE (LWORD(xpluse));
	respBuf[6] = LBYTE (HWORD(ypluse));
	respBuf[7] = HBYTE (LWORD(ypluse));
	respBuf[8] = LBYTE (LWORD(ypluse));
	respBuf[9] = 0;
	respBuf[10] = LBYTE (HWORD(zpluse));
	respBuf[11] = HBYTE (LWORD(zpluse));
	respBuf[12] = LBYTE (LWORD(zpluse));
	respBuf[13] = 0;
	respBuf[14] = LBYTE (HWORD(synpluse));
	respBuf[15] = HBYTE (LWORD(synpluse));
	respBuf[16] = LBYTE (LWORD(synpluse));
	respBuf[17] = 0;
	respBuf[18] = 0;
	respBuf[19] = 0;	
	respBuf[20] = 0;
	HostResponse(respBuf, 21);
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
 *发送吸试剂结果码，Z轴为最低端时的脉冲
 *******************************************************************/
STATIC VOID SendSubMessage(VOID)
{
	INT32 xpluse,ypluse,zpluse,synpluse;
	BYTE respBuf[21];
	xpluse = StepMotorGetPos (&(MainState.motors[MOTOR_X]));
	ypluse = StepMotorGetPos (&(MainState.motors[MOTOR_Y]));
	zpluse = MainState.Parameter.zLowestPulse;
	synpluse = StepMotorGetPos (&(MainState.motors[MOTOR_SYN]));
	respBuf[0] = 0;
	respBuf[1] = 0;
	respBuf[2] = MainState.Parameter.wrkCode;
	respBuf[3] = LBYTE (HWORD(xpluse));
	respBuf[4] = HBYTE (LWORD(xpluse));
	respBuf[5] = LBYTE (LWORD(xpluse));
	respBuf[6] = LBYTE (HWORD(ypluse));
	respBuf[7] = HBYTE (LWORD(ypluse));
	respBuf[8] = LBYTE (LWORD(ypluse));
	respBuf[9] = 0;
	respBuf[10] = LBYTE (HWORD(zpluse));
	respBuf[11] = HBYTE (LWORD(zpluse));
	respBuf[12] = LBYTE (LWORD(zpluse));
	respBuf[13] = 0;
	respBuf[14] = LBYTE (HWORD(synpluse));
	respBuf[15] = HBYTE (LWORD(synpluse));
	respBuf[16] = LBYTE (LWORD(synpluse));
	respBuf[17] = 0;
	respBuf[18] = 0;
	respBuf[19] = 0;	
	respBuf[20] = 0;
	HostResponse(respBuf, 21);
}
/********************************************************************
 *结果码返回函数
 *******************************************************************/
VOID SendResultMessage(VOID)
{
	MainState.stateManageTop = STATE0_READY;
	switch(MainState.Parameter.receiveCmd)
	{
		case SAMPLEHOST_CMD_VERSION       	 :  SendVersionMessage();    	break;
		case SAMPLEHOST_CMD_GETINFO       	 :    	break;
		case SAMPLEHOST_CMD_SETINFO       	 :     	break;
		case SAMPLEHOST_CMD_PARAINFO      	 :  SendParameterMessage();     break;
		case SAMPLEHOST_CMD_PARASET			 :  SendSetParameterMessage(); 	break;
		case SAMPLEHOST_CMD_WRKSAVE          :  SendWorkSaveMessage();      break;
		case SAMPLEHOST_CMD_MOTOR_INQUIRE	 : 	SendMotorPos(); 			break;   	//发送电机的坐标位置	
		case SAMPLEHOST_CMD_MOTOR_ENABLE	 : 	
		case SAMPLEHOST_CMD_MOTOR_RESET		 : 		
		case SAMPLEHOST_CMD_MOTOR_TOZERO	 :  		
		case SAMPLEHOST_CMD_MOTOR_MOVREL	 :  		
		case SAMPLEHOST_CMD_MOTOR_MOVABS	 : 	
		case SAMPLEHOST_CMD_MOTOR_MOVWRK	 : 		
		case SAMPLEHOST_CMD_MOTOR_SUBDRA	 :    			    	
		case SAMPLEHOST_CMD_MOTOR_MOVXY		 :  			 		
		case SAMPLEHOST_CMD_MOTOR_ZAXISDRA	 : 		
		case SAMPLEHOST_CMD_MOTOR_ZAXISDRABUF: 		
		case SAMPLEHOST_CMD_MOTOR_ZAXISCLE	 : 	SendCombinationMessage();	break;
		case SAMPLEHOST_CMD_MOTOR_ZAXISSUBBUF: 		
		case SAMPLEHOST_CMD_MOTOR_ZAXISSUB	 :	SendSubMessage();	break;
		case SAMPLEHOST_CMD_PUMPSET       	 : 		break;
		case SAMPLEHOST_CMD_PUMPGET       	 : 		break;
		case SAMPLEHOST_CMD_VALVESET      	 : 		break;
		case SAMPLEHOST_CMD_VALVEGET      	 : 		break;
		case SAMPLEHOST_CMD_IOSET			 : 		break;
		case SAMPLEHOST_CMD_IOGET			 : 		break;
		case SAMPLEHOST_CMD_OPTOGET			 : 		break;
		case SAMPLEHOST_CMD_FAULT			 : 		break;
		case SAMPLEHOST_CMD_STOP			 :	SendStopMessage();			break;
		default:break;
	}
}






