#ifndef _EEROM_DATA_H_
#define _EEROM_DATA_H_

#include <cstdio>
#include <cstdint>
#include "hexdump.h"

struct EEROMData
{
    uint8_t diffFixData[2][256];
    uint8_t caliLevel[200];
    uint8_t fpgaAlert[40];
    uint8_t productName[20];
    uint8_t userName[12];

    void print()
    {
        printf(
            "[EEROMData]\n"
            "diffFixData[0]:\n");
        hexdump(diffFixData[0], 256);
        printf("diffFixData[1]:\n");
        hexdump(diffFixData[1], 256);
        printf("caliLevel:\n");
        hexdump(caliLevel, 200);
        printf("fpgaAlert:\n");
        hexdump(fpgaAlert, 40);
        printf(
            "productName = %s\n"
            "userName = %s\n"
            "\n\n",
            productName, userName);
    }
};

#endif // _EEROM_DATA_H_