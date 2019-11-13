/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Host.h
 *******************************************************************/
#ifndef _BOARD_HOST_H_
#define _BOARD_HOST_H_
/********************************************************************
 * Host defined
 *******************************************************************/
#include "Datatypes.h"
#include "Console.h"
#include "Uart.h"
#include "Board.h"
#include "Buffer.h"
#include "Bits.h"
#include <string.h>
#define NOTHING             (0x00)
#define FIRST_DATA          (0x01)
#define SECOND_DATA         (0x02)
#define THIRD_DATA          (0x03)
#define FOURTH_DATA         (0x04)
#define FIFTH_DATA          (0x05)
#define SIXTH_DATA          (0x06)
#define SEVENTH_DATA        (0x07)
#define EIGHTH_DATA         (0x08)
#define NINTH_DATA          (0x09)
#define TENTH_DATA          (0x0A)
#define ELEVENTH_DATA       (0x0B)
#define TWELFTH_DATA        (0x0C)

#define ACK_SUCCESS         (0x0D)
#define ACK_ERR             (0x0E)
#define ACK_DATA_DEFAULT    (0x0000)


#define FIRST_IDENTIFY      (0x1)
#define SECOND_IDENTIFY     (0x0)
#define THIRD_IDENTIFY      (0x0)
#define COMMU_RX_LEN		(64)
#define COMMU_TX_LEN		(64)
/********************************************************************
 *
 *******************************************************************/
//#define DEBUG
typedef struct {
    UINT8 hDataByte7_6          :2;
    UINT8 sequence              :4;
    UINT8 check                 :1;
    UINT8 firstByteIdentify     :1;
    UINT8 hDataByte5_0          :6;
    UINT8 lDataByte7            :1;    
    UINT8 secondByteIdentify    :1;
    UINT8 lDataByte6_0          :7;
    UINT8 thirdByteIdentify     :1;
    
}BoardCommuFrame;

typedef UINT32 (*CMDPARSER)(BYTE cmd);
typedef UINT32 (*DATAPARSER)(BoardCommuFrame data); 

typedef struct {
	Uart*  uart;
	BYTE   txArray[COMMU_RX_LEN];
	BYTE   rxBufferArray[COMMU_TX_LEN];
	Buffer rxBuffer;
	CMDPARSER cmdParser;
    DATAPARSER dataParder;
	BoardCommuFrame dataFrame;
    BYTE cmdFrame;
}CommuObj;
/********************************************************************
 *
 *******************************************************************/
VOID BoardCmdAck     (UINT8 sequnce,UINT16 data);
VOID BoardCommuInit    (VOID);
UINT16 GetSequence(BYTE* buffer);
UINT16 GetData(BYTE* buffer);
VOID ObjCmdProcess (VOID);
VOID ObjDataProcess (INT16* data,UINT8* sequence);
VOID ObjCmdParserRegister (CMDPARSER parser);
UINT8 HostGetConsoleTag(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif //
