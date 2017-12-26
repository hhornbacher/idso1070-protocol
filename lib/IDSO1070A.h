#ifndef _IDSO1070A_H_
#define _IDSO1070A_H_

#include <cstdio>
#include <cstdint>

#include "TimeBase.h"

struct IDSO1070A
{
    const uint16_t maxPWM = 4095;
    char date[9];
    uint8_t batteryLevel;
    bool channel1;
    bool channel2;
    uint8_t receiveFreqDivStatus;
    uint32_t freqDiv;

    TimeBase timeBase;

    void print()
    {
        printf(
            "[IDSO1070A]\n"
            "date = %s\n"
            "freqDiv = %d\n"
            "batteryLevel = %d\n"
            "channel1 = %d\n"
            "channel2 = %d\n"
            "\n\n",
            date, freqDiv, batteryLevel, channel1, channel2);
    }
};

#endif // _IDSO1070A_H_