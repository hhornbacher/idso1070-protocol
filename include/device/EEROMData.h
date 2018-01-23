#ifndef _EEROM_DATA_H_
#define _EEROM_DATA_H_

#include "base.h"

struct EEROMData
{
    uint8_t diffFixData[2][256];
    uint16_t caliLevel[100];
    uint8_t fpgaAlert[40];
    uint8_t productName[20];
    uint8_t userName[12];
};

#endif // _EEROM_DATA_H_