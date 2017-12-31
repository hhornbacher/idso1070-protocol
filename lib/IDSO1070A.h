#ifndef _IDSO1070A_H_
#define _IDSO1070A_H_

#include <cstdio>
#include <cstdint>

#include "enums.h"

#define IDSO1070A_MAX_SAMPLE 248
#define IDSO1070A_MAX_PWM 4095
#define IDSO1070A_MEMORY_DEPTH 2000
#define IDSO1070A_SAMPLES_COUNT_PER_PACKET 500

struct IDSO1070A
{

    struct Trigger
    {
        bool isHold = false;
        TriggerMode mode = TRIGMODE_AUTO;
        TriggerChannel channel = TRIGCHAN_CH1;
        TriggerSlope slope = TRIGSLOPE_RISING;
        uint16_t level = 0;
        double xPosition = 0.5;
        uint16_t innerTriggerPWM[4];
        uint16_t outerTriggerPWM[2];

        uint16_t getBottomPWM();
        uint16_t getTopPWM();
        void print();
        void setTriggerLevel(uint16_t i);
    };

    struct Channel
    {
        char *name;
        bool enabled = true;
        VoltageDiv verticalDiv;
        InputCoupling coupling;
        int verticalPosition;
        uint16_t pwmArray[9][2];

        void print();
        void setVerticalPosition(int i);
    };

    Channel channel1;
    Channel channel2;
    Channel *selectedChannel = &channel1;
    Trigger trigger;

    uint8_t batteryLevel;

    char date[9];
    uint8_t receiveFreqDivStatus;
    uint32_t freqDiv;

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