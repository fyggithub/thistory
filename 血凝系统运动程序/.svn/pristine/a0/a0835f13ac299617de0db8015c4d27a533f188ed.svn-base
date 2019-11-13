#ifndef AD7689_H
#define AD7689_H
#include "Spi.h"
#include "Datatypes.h"
enum{CH1_6,CH7_12} ;
typedef struct __AD7689{
	UINT16 id;
	UINT16 ResConfig;
    UINT16* BuffPtr;
    UINT16 DataLen;
    UINT16 IoCnv;
    UINT16 temp[6];
}AD7689;
VOID AD7689RegConfig(AD7689* _AD7689Def,Spi*_spi);
VOID AD7689Get(UINT16* ch);
#endif
