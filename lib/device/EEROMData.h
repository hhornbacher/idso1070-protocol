#ifndef _EEROM_DATA_H_
#define _EEROM_DATA_H_

#include <cstdio>
#include <cstdint>

#include "../util/hexdump.h"

struct EEROMData
{
    uint8_t diffFixData[2][256];
    uint8_t caliLevel[200];
    uint8_t fpgaAlert[40];
    uint8_t productName[20];
    uint8_t userName[12];

    void print();
};

#endif // _EEROM_DATA_H_