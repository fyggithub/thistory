/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Message.c
 *******************************************************************/
#include "Message.h"
#include "Console.h"
#include <string.h>
/********************************************************************
 *
 *******************************************************************/
#define MSG_QUEUE_MAX (8)
typedef struct __MessageQueue{
	Message message[MSG_QUEUE_MAX];
	UINT16  idxr;
	UINT16  idxw;
	MSG_HANDLER handler;
}MessageQueue;

STATIC MessageQueue msgQueue;
/********************************************************************
 * copy message in
 *******************************************************************/
STATIC VOID CopyToMessage(Message* msg, UINT16 idx)
{
	if (msg && idx < MSG_QUEUE_MAX){
		Message* msgQ = &(msgQueue.message[idx]);
		msg->id = msgQ->id;
		msg->p1 = msgQ->p1;
		msg->p2 = msgQ->p2;
		msg->p3 = msgQ->p3;
		msg->p4 = msgQ->p4;
	}
}
 /********************************************************************
 * copy message out
 *******************************************************************/
STATIC VOID CopyFromMessage(Message* msg, UINT16 idx)
{
	if (msg && idx < MSG_QUEUE_MAX){
		Message* msgQ = &(msgQueue.message[idx]);
		msgQ->id = msg->id;
		msgQ->p1 = msg->p1;
		msgQ->p2 = msg->p2;
		msgQ->p3 = msg->p3;
		msgQ->p4 = msg->p4;
	}
}
/********************************************************************
 * just post in mainloop
 * if no free node to post, will return false && error!
 *******************************************************************/
BOOL MessagePost(Message* msg)
{
	__disable_irq();
	BOOL ret;
	UINT16 idxr = msgQueue.idxr;
	UINT16 idxw = msgQueue.idxw;
	// if there are some free node
	idxw = (idxw+1) % MSG_QUEUE_MAX;
	if ( idxr != idxw && msg){
		idxw = msgQueue.idxw;
		CopyFromMessage(msg, idxw);
		msgQueue.idxw = (idxw+1) % MSG_QUEUE_MAX;

		// EventSet(EV_MESSAGE);
		ret = TRUE;
	} else {
		ret = FALSE;
	}
	__enable_irq();

//	if(ret){
//		DebugLog(0, "Msg Post - (%d : %d, %d, %d, %d)",
//			msg->id, msg->p1, msg->p2, msg->p3, msg->p4);
//	}
//	else {
//		if (!msg)
//			DebugError(0, "Msg Post Error - Null Pointer");
//		else
//			DebugError(0, "Msg Post Error - No Free Room");

//	}

	return ret;
}
/********************************************************************
 * just get in mainloop
 * if no message, return False && clear event
 *******************************************************************/
BOOL MessageGet (Message* msg)
{
	__disable_irq();
	BOOL ret;
	UINT16 idxr = msgQueue.idxr;
	UINT16 idxw = msgQueue.idxw;
	// if there are some message
	if ( idxr != idxw && msg){
		CopyToMessage(msg, idxr);
		msgQueue.idxr = (idxr+1) % MSG_QUEUE_MAX;
		ret = TRUE;
	} else {
		// EventClear(EV_MESSAGE);
		ret = FALSE;
	}
	__enable_irq();
	return ret;
}
/********************************************************************
 * Message Queue Init
 *******************************************************************/
VOID MessageQueueInit(VOID)
{
	msgQueue.idxr = msgQueue.idxw = 0;
	memset(msgQueue.message, 0, sizeof(msgQueue.message));
	msgQueue.handler = NULL;
	DebugLog (0, "Message Queue Init.");
}

/********************************************************************
 * Message Queue Init
 *******************************************************************/

VOID MessageRegisterHandler(MSG_HANDLER handler)
{
	if(msgQueue.handler == NULL && handler != NULL){
		msgQueue.handler = handler;
		DebugLog(0, "Message Handler Register - %08X", handler);
	}
}

/********************************************************************
 * Message Queue Init
 *******************************************************************/

VOID MessageQueueProcess(VOID)
{
	Message message;
	while(MessageGet(&message)){
		msgQueue.handler(&message);
	}
}

/********************************************************************
 * End of File
 *******************************************************************/
