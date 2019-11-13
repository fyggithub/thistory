#include "SendResultCode.h"

/********************************************************************
 *
 *******************************************************************/
VOID ResponseState(VOID)
{
	StepMotor* dispMotor = &(loadApp.motors[MOTOR_DISP]);
	INT16 stepAbs = (INT16)StepMotorGetPos(dispMotor);

	UINT8 data[11];
	data[0] = data[1] = 0;
	data[2] = (UINT8)(loadApp.whichWorkHoleOn);
	data[3] = (stepAbs >= 0 ? 0 : -1);
	data[4] = HBYTE(stepAbs);
	data[5] = LBYTE(stepAbs);
	data[6] = 0;
	data[7] = data[8] = data[9] = 0;
//	data[10] = GetOptoState();
	data[10] = 0xff;
	HostResponse (data, 11);
}

VOID ResponseSuccess(VOID)
{
	UINT8 data[1];
	data[0] = 0;
	HostResponse (data, 1);
}
/********************************************************************
 *发送查询参数的值
 *******************************************************************/
VOID SendParameterMessage(VOID)
{
	INT32 value1,value2,value3,value4;
	BYTE sendBuf[25];	
	INT16 id1 = loadApp.codeID1;
	INT16 id2 = loadApp.codeID2;
	INT16 id3 = loadApp.codeID3;
	INT16 id4 = loadApp.codeID4;
	
	if(id1 >= CFG_LOAD_POSLMT_VNEG)
		value1 = ConfigGetDword(id1);
	else
		value1 = 0;
	if(id2 >= CFG_LOAD_POSLMT_VNEG)
		value2 = ConfigGetDword(id2);
	else
		value2 = 0;
	if(id3 >= CFG_LOAD_POSLMT_VNEG)
		value3 = ConfigGetDword(id3);
	else
		value3 = 0;
	if(id4 >= CFG_LOAD_POSLMT_VNEG)
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




