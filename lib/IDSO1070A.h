#ifndef _IDSO1070A_H_
#define _IDSO1070A_H_

#include <cstdio>
#include <cstdint>

#include "enums.h"

struct IDSO1070A
{
    struct Trigger
    {
        bool isHold;
        TriggerChannel channel;
        int level;
        TriggerMode mode;
        TriggerSlope slope;
        double xPosition;
    };

    struct Channel
    {
        char *name;
        bool enabled;
        VoltageDiv verticalDiv;
        InputCoupling coupling;
        int verticalPosition;

        void print()
        {
            printf(
                "[Channel]\n"
                //"name = %s\n"
                "enabled = %d\n"
                "verticalDiv = %d\n"
                "coupling = %d\n"
                "\n\n",
                /*name,*/ enabled, verticalDiv, coupling);
        }
    };
    const size_t samplesCountPerPacket = 500;
    const size_t memoryDepth = 2000;
    const uint16_t maxPWM = 4095;
    char date[9];
    uint8_t batteryLevel;
    Channel channel1;
    Channel channel2;
    Trigger trigger;
    uint8_t receiveFreqDivStatus;
    uint32_t freqDiv;
    uint16_t pwmArray[9][2];

    TimeBase timeBase;

    CaptureMode captureMode;
    ScopeMode scopeMode;

    bool isSampleRate200Mor250M();
    size_t getSamplesNumberOfOneFrame()
    {
        return (memoryDepth == 2000 && timeBase == HDIV_5uS) ? 2500 : memoryDepth;
    };
    uint8_t getEnabledChannelsCount()
    {
        if (channel1.enabled && channel2.enabled)
            return 2;
        else if (channel1.enabled || channel2.enabled)
            return 1;
        return 0;
    }
    uint8_t getPacketsNumber()
    {
        int enabledChannelsCount = getEnabledChannelsCount();
        return enabledChannelsCount == 0 ? 0 : (enabledChannelsCount * getSamplesNumberOfOneFrame()) / samplesCountPerPacket;
    }

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