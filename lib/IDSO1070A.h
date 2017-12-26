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
        uint16_t innerTriggerPWM[4];
        uint16_t outerTriggerPWM[2];

        uint16_t getBottomPWM();
        uint16_t getTopPWM();
        void print();
    };

    struct Channel
    {
        char *name;
        bool enabled;
        VoltageDiv verticalDiv;
        InputCoupling coupling;
        int verticalPosition;

        void print();
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
    size_t getSamplesNumberOfOneFrame();
    uint8_t getEnabledChannelsCount();
    uint8_t getPacketsNumber();
    TimeBase getTimebaseFromFreqDiv();

    void print();
};

#endif // _IDSO1070A_H_