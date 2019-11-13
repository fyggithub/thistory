/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Message.h
 *******************************************************************/
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
/********************************************************************
 * Message defined
 *******************************************************************/
#include "Datatypes.h"
#include "Bits.h"
#include "Board.h"

/********************************************************************
 *
 *******************************************************************/

typedef struct __Message{
	UINT32 id; // message id
	INT32  p1; // parameter 1
	INT32  p2; // parameter 2
	INT32  p3; // parameter 3
	INT32  p4; // parameter 4
}Message;

typedef VOID (*MSG_HANDLER)(Message* msg);
/********************************************************************
 * Message Method
 *******************************************************************/
BOOL MessagePost(Message* msg); // just post in mainloop
BOOL MessageGet (Message* msg); // just get  in mainloop
VOID MessageQueueInit(VOID);	// Message Queue Init
VOID MessageRegisterHandler(MSG_HANDLER handler);
VOID MessageQueueProcess(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _MESSAGE_H_
