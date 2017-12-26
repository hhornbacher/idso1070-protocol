#ifndef _IDSO1070A_H_
#define _IDSO1070A_H_

#include <cstdio>
#include <cstdint>

#include "VoltageDiv.h"
#include "TimeBase.h"

struct IDSO1070A
{
    struct Channel
    {
        char *name;
        bool enabled;
        VoltageDiv verticalDiv;

        void print()
        {
            printf(
                "[Channel]\n"
                //"name = %s\n"
                "enabled = %d\n"
                "verticalDiv = %d\n"
                "\n\n",
                /*name,*/ enabled, verticalDiv);
        }
    };

    const uint16_t maxPWM = 4095;
    char date[9];
    uint8_t batteryLevel;
    Channel channel1;
    Channel channel2;
    uint8_t receiveFreqDivStatus;
    uint32_t freqDiv;

    TimeBase timeBase;

    void print()
    {
        printf(
            "[IDSO1070A]\n"
            "date = %s\n"
            "freqDiv = %d\n"
            "batteryLevel = %d\n",
            date, freqDiv, batteryLevel);
        channel1.print();
        channel2.print();
        printf("\n\n");
    }
};

#endif // _IDSO1070A_H_