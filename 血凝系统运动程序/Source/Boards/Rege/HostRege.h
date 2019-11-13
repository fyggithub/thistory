#ifndef __HOSTREGE_H__
#define __HOSTREGE_H__

/********************************************************************
 *
 *******************************************************************/
#include "Host.h"
#include "Console.h"
#include "Message.h"
#include "Datatypes.h"
#include "Board.h"
#include "Regent.h"

/********************************************************************
 *
 *******************************************************************/
#define HOST_ADDR_REGE	(0x08)

#define REGEHOST_CMD_VERSION		(0x10)
#define REGEHOST_CMD_GETINFO		(0x11)
#define REGEHOST_CMD_SETINFO		(0x12)
#define REGEHOST_CMD_PARAINFO		(0x13)
#define REGEHOST_CMD_PARASET		(0x14)
#define REGEHOST_CMD_WRKSAVE		(0x15)
#define REGEHOST_CMD_MOTOR_ENABLE	(0x20)
#define REGEHOST_CMD_MOTOR_RESET	(0x21)
#define REGEHOST_CMD_MOTOR_TOZERO	(0x22)
#define REGEHOST_CMD_MOTOR_MOVREL	(0x23)
#define REGEHOST_CMD_MOTOR_MOVABS	(0x24)
#define REGEHOST_CMD_MOTOR_MOVWRK	(0x25)
#define REGEHOST_CMD_MOTOR_ZAIXDET  (0x26)
#define REGEHOST_CMD_MOTOR_SUBDRA   (0x27)
#define REGEHOST_CMD_MOTOR_INQUIRE  (0x28)
#define REGEHOST_CMD_MOTOR_MOVXY    (0x29)
#define REGEHOST_CMD_MOTOR_ZAXISSUB (0x2A)
#define REGEHOST_CMD_MOTOR_ZAXISDRA (0x2B)
#define REGEHOST_CMD_MOTOR_ZAXISCLE (0x2C)
#define REGEHOST_CMD_PUMPSET    	(0x18)
#define REGEHOST_CMD_PUMPGET    	(0x19)
#define REGEHOST_CMD_VALVESET   	(0x1A)
#define REGEHOST_CMD_VALVEGET   	(0x1B)
#define REGEHOST_CMD_IOSET			(0x1C)
#define REGEHOST_CMD_IOGET			(0x1D)
#define REGEHOST_CMD_OPTOGET		(0x1E)
#define REGEHOST_CMD_FAULT	    	(0xF0)
#define REGEHOST_CMD_TEMP	    	(0x2F)
#define REGEHOST_CMD_DEALY	    	(0x0D)
#define REGEHOST_CMD_STOP	    	(0x0E)
/********************************************************************
 *
 *******************************************************************/
VOID HostRegeInit(VOID);


#endif


