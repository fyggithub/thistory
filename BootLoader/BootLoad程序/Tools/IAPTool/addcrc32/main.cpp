#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char                CHAR;
typedef signed char         INT8;
typedef signed short int    INT16;
typedef signed int          INT32;
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned int        UINT32;
typedef int                 BOOL;
typedef float               FLOAT;
typedef double              DOUBLE;

#define TRUE    1
#define FALSE   0
#define VOID    void

UINT32 CrcValue = 0xFFFFFFFF;
UINT8 buffer[1024*1024];

BOOL CrcCalculate(UINT8 *buffer, UINT32 dateLen, UINT32 * crcValue)
{
    UINT32 POLY = 0x4C11DB7;
    UINT32 index = 0;
    UINT32 *pbuff = (UINT32 *)buffer, len = dateLen >> 2;

    while (len > 0)
    {
        index = 0;
        CrcValue = CrcValue ^ (*pbuff);
        do
        {
            if ((CrcValue & 0x80000000) == 0x80000000)
            {
                CrcValue = (CrcValue << 1) ^ POLY;
            }
            else
            {
                CrcValue <<= 1;
            }
            index++;
        } while (index < 32);
        pbuff++;
        len--;
    }
    *crcValue = CrcValue;
    return TRUE;
}

void addCrc32(const char* input, const char* output)
{
    FILE* fpr, *fpw;

    fpr = fopen(input, "rb+");
    fpw = fopen(output, "wb+");
    if (fpr != NULL && fpw != NULL){

        fseek(fpr, 0, SEEK_END);
        UINT32 length = ftell(fpr);
        fseek(fpr, 0, SEEK_SET);

        fread(buffer, length, 1, fpr);

        UINT32 crc;
        CrcValue = 0xFFFFFFFF;
        CrcCalculate(buffer+32, length-32, &crc);
        *((UINT32*)buffer) = crc;
        *((UINT32*)(buffer+8)) = length;

        fwrite(buffer, length, 1, fpw);

        fclose(fpr);
        fclose(fpw);
    }
}

int main(int argc, char *argv[])
{
#if 1
    if (argc >= 3){
        addCrc32(argv[1], argv[2]);
    }
#else
     addCrc32("test.bin", "testcrc.bin");
#endif
    return 0;
}
