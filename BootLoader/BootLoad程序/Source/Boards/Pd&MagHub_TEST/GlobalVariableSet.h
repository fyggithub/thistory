/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 
 * 
 * GlobalVariableSet.h
 *******************************************************************/
 #ifndef GLOBALVARIABLESET_H
 #define GLOBALVARIABLESET_H
 #include "DataTypes.h"
/********************************************************************
*
*******************************************************************/
enum{
    GLOBAL_LED_ON   = 0x01,
    GLOBAL_DIV_DATA = 0x02,
    GLOBAL_ON_DATA  = 0x04,
    GLOBAL_OFF_DATA = 0x08,
    GLOBAL_MAG_DATA = 0x10,
		GLOBAL_PD_STORE = 0x20,
		GLOBAL_PD_DEBUG = 0x40,
};
VOID GloVarInit(VOID);
VOID GloVarSet(UINT32 gloVarMask);
VOID GloVarClear(UINT32 gloVarMask);
UINT32 GloVarGet(VOID);
BOOL GloVarHas(UINT32 gloVarMask);
#endif
/********************************************************************
 * End of File
 *******************************************************************/
