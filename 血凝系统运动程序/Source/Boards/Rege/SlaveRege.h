#ifndef __SlaveRege_H__
#define __SlaveRege_H__

#include "Datatypes.h"


#define SLAVE_ACK_SUCCESS	(0x00)
#define SLAVE_ACK_ERR_LEN	(0x01)
#define SLAVE_ACK_ERR_CRC	(0x02)
#define SLAVE_ACK_ERR_ADDR	(0x03)
#define SLAVE_FRAME_DATALEN	(64)

typedef struct __SlaveFrame{
	UINT16 number;
	UINT8  type;
	UINT8  command;
	UINT8  sub;
	BYTE   data[SLAVE_FRAME_DATALEN];
	UINT16 dataLen;
}SlaveFrame;
typedef UINT32 (*SLAVEPARSER)(SlaveFrame* frame); // return ack

/********************************************************************
 * Slave Method
 *******************************************************************/
VOID SlaveAck     (UINT32 ack);
VOID SlaveResponse(BYTE* data, UINT16 len);
VOID SlaveSend(UINT16 main_sub,BYTE* frame, UINT16 len);
VOID SlaveInit    (VOID);
VOID SlaveProcess (VOID);
VOID SlaveParserRegister (UINT16 addr, SLAVEPARSER parser);
UINT8 SlaveGetConsoleTag(VOID);

VOID RegeSendSlaveCmdMode(INT16 setpwm,INT16 heattime,INT16 stoptime);
VOID RegeSendCmdTemp(VOID);
VOID RegeSendCmdCtlPid(INT16 state);


#endif



