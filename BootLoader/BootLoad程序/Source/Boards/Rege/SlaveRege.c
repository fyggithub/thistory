#include "SlaveRege.h"
#include "Console.h"
#include "Message.h"
#include "Host.h"
#include "Board.h"
#include <string.h>
/********************************************************************
 *
 *******************************************************************/
#define SLAVE_IDENTIFY	  (0xE1C3)
#define SLAVE_ADDR		  (0x08)
#define SLAVE_TYPE_ASK	  (0x01)
#define SLAVE_TYPE_ACK	  (0x11)
#define SLAVE_TYPE_RESULT (0x12)
#define SLAVE_MAIN_CMD    (0xC9)
/********************************************************************
 *
 *******************************************************************/
#define SLAVE_RX_LEN		(64)
#define SLAVE_TX_LEN		(64)
#define SLAVE_PARSER_MAX	(2)
/********************************************************************
 *
 *******************************************************************/
typedef struct __SlaveParser{
	UINT16 addr;
	SLAVEPARSER parser;
}SlaveParser;

typedef struct __Slave{
	Uart*  uart;
	BYTE   txArray[SLAVE_TX_LEN];
	BYTE   rxArray[SLAVE_RX_LEN];
	BYTE   rxBufferArray[SLAVE_RX_LEN];
	Buffer rxBuffer;
	SlaveParser parsers[SLAVE_PARSER_MAX];
	UINT16 parserNum;
	SlaveFrame lastFrame;
	INT16 tempVlave;
}Slave;

STATIC Slave slave;
STATIC UINT8 tag;

/********************************************************************
 *
 *******************************************************************/
STATIC UINT16 CalcCrc16(BYTE* buff, UINT16 len)
{
	CONST UINT16 CRC_QUANTIC = 0xA001;
	UINT16 crc = 0;
	for(UINT16 i = 0; i<len; i++){
		crc = crc^(*buff);
		buff++;
		for(UINT8 j=0; j<8; j++){
			if((crc&0x0001) == 1){
				crc = crc >> 1;
				crc = crc ^ CRC_QUANTIC;
			}else{
				crc = crc >> 1;
			}
		}
	}
	return crc;
}
/********************************************************************
 *
 *******************************************************************/
INT16 GetTemperature(VOID)
{
	return slave.tempVlave;
}
/********************************************************************
 *
 *******************************************************************/
VOID SlaveSend(UINT16 main_sub,BYTE* frame, UINT16 len)
{
	UINT16 frameLen = len+6;
	BYTE respBuf[SLAVE_TX_LEN];
	respBuf[0] = HBYTE (SLAVE_IDENTIFY);
	respBuf[1] = LBYTE (SLAVE_IDENTIFY);
	respBuf[2] = HBYTE (frameLen);
	respBuf[3] = LBYTE (frameLen);
	respBuf[4] = HBYTE (0);
	respBuf[5] = LBYTE (0);
	respBuf[6] = SLAVE_ADDR;
	respBuf[7] = SLAVE_TYPE_ASK;
	respBuf[8] = main_sub;
	respBuf[9] = LBYTE(0);
	memcpy (respBuf+10, frame, len);
    UINT16 crc = CalcCrc16 (respBuf+4, frameLen);
	respBuf[frameLen+4] = HBYTE (crc);
	respBuf[frameLen+5] = LBYTE (crc);
	UartSend (slave.uart, respBuf,  frameLen+6);
	DebugHex (tag, respBuf,  frameLen+6);
}
/********************************************************************
 *
 *******************************************************************/
VOID SlaveInit    (VOID)
{
	tag = DebugRegister ("sla");
	slave.uart = BoardGetSlaveUart();
	BufferCreate (&(slave.rxBuffer), slave.rxBufferArray, SLAVE_RX_LEN);
	UartInit (slave.uart, &(slave.rxBuffer), slave.txArray, SLAVE_RX_LEN);
	DebugLog(tag, "Slave Init Finished.");
}
/********************************************************************
 *
 *******************************************************************/
VOID SlaveProcess (VOID)
{
	UINT16 len = BufferLength (&(slave.rxBuffer));
	if (len >= 4) {
		BYTE header[4];
		BufferPeek (&(slave.rxBuffer), header, 4);
		UINT16 identify = MAKE_WORD (header[0], header[1]);
		if (identify == SLAVE_IDENTIFY) {
			UINT16 frameLen = MAKE_WORD (header[2], header[3]);
			if (len >= frameLen+6 ){
				len = BufferPop (&(slave.rxBuffer),
					slave.lastFrame.data, frameLen+6);				
				if((slave.lastFrame.data[7] == SLAVE_TYPE_RESULT) && (slave.lastFrame.data[8] == SLAVE_MAIN_CMD))
				{  //如果是结果帧，则存储
					BYTE temp[2];
					temp[0] = slave.lastFrame.data[10];
					temp[1] = slave.lastFrame.data[11];
					HostResponse(temp,2);	
				}  
				if (len == frameLen+6){
					DebugHex(tag,slave.lastFrame.data,len);
				}
			}
		} else {
			// throw 1 byte
			BufferPop(&(slave.rxBuffer), header, 1);
		}
	}
}
/********************************************************************
 *
 *******************************************************************/
BYTE *GetSlaveTemp(VOID)
{
	BYTE *temp;
	temp[0] = HBYTE (slave.tempVlave);
	temp[1] = LBYTE (slave.tempVlave);
	return temp;
}
/********************************************************************
 *配置液面检测板的工作模式
 *******************************************************************/
VOID RegeSendSlaveCmdMode(INT16 setpwm,INT16 heattime,INT16 stoptime)
{
	BYTE buff[6];
	buff[0] = HBYTE (setpwm);
	buff[1] = LBYTE (setpwm);
	buff[2] = HBYTE (heattime);
	buff[3] = LBYTE (heattime);
	buff[4] = HBYTE (stoptime);
	buff[5] = LBYTE (stoptime);
	SlaveSend(0xC8,buff,6);
}
/********************************************************************
 *查询试剂针温度
 *******************************************************************/
VOID RegeSendCmdTemp(VOID)
{
	BYTE buff[1];
	buff[0] = 0;
	SlaveSend(0xC9,buff, 1);
}
/********************************************************************
 *告诉液面检测开启或关闭PID控制  0-关闭    1-开启
 *******************************************************************/
VOID RegeSendCmdCtlPid(INT16 state)
{
	BYTE buff[1];
	buff[0] = state;
	SlaveSend(0xCA,buff, 1);
}

/********************************************************************
 * END OF FILE
 *******************************************************************/






