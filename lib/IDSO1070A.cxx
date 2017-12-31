#include "IDSO1070A.h"

bool IDSO1070A::isSampleRate200Mor250M()
{
    return timeBase <= HDIV_1uS;
}

size_t IDSO1070A::getSamplesNumberOfOneFrame()
{
    return (IDSO1070A_MEMORY_DEPTH == 2000 && timeBase == HDIV_5uS) ? 2500 : IDSO1070A_MEMORY_DEPTH;
};
uint8_t IDSO1070A::getEnabledChannelsCount()
{
    if (channel1.enabled && channel2.enabled)
        return 2;
    else if (channel1.enabled || channel2.enabled)
        return 1;
    return 0;
}
uint8_t IDSO1070A::getPacketsNumber()
{
    int enabledChannelsCount = getEnabledChannelsCount();
    return enabledChannelsCount == 0 ? 0 : (enabledChannelsCount * getSamplesNumberOfOneFrame()) / IDSO1070A_SAMPLES_COUNT_PER_PACKET;
}

TimeBase IDSO1070A::getTimebaseFromFreqDiv()
{
    switch (freqDiv)
    {
    case 0:
        return HDIV_2uS;
    case 1:
        return HDIV_5uS;
    case 4:
        return HDIV_10uS;
    case 9:
        return HDIV_20uS;
    case 24:
        return HDIV_50uS;
    case 49:
        return HDIV_100uS;
    case 99:
        return HDIV_200uS;
    case 249:
        return HDIV_500uS;
    case 499:
        return HDIV_1mS;
    case 999:
        return HDIV_2mS;
    case 2499:
        return HDIV_5mS;
    case 4999:
        return HDIV_10mS;
    case 9999:
        return HDIV_20mS;
    case 24999:
        return HDIV_50mS;
    case 49999:
        return HDIV_100mS;
    case 99999:
        return HDIV_200mS;
    case 249999:
        return HDIV_500mS;
    case 499999:
        return HDIV_1S;
    case 999999:
        return HDIV_2S;
    case 2499999:
        return HDIV_5S;
    case 4999999:
        return HDIV_10S;
    case 9999999:
        return HDIV_20S;
    case 24999999:
        return HDIV_50S;
    case 49999999:
        return HDIV_100S;
    case 99999999:
        return HDIV_200S;
    case 249999999:
        return HDIV_500S;
    default:
        return HDIV_1mS;
    }
}

void IDSO1070A::print()
{
    printf("[IDSO1070A]\n");
    channel1.print();
    channel2.print();
    trigger.print();
    printf("batteryLevel = %d\n", batteryLevel);
    printf("date = %s\n", date);
    printf("receiveFreqDivStatus = 0x%02x\n", receiveFreqDivStatus);
    printf("freqDiv = %d\n", freqDiv);
    printf("timeBase = %d\n", timeBase);
    printf("captureMode = %d\n", captureMode);
    printf("scopeMode = %d\n", scopeMode);
    printf("\n\n");
}

void IDSO1070A::Channel::print()
{
    printf("[Channel]\n");
    printf("enabled = %d\n", enabled);
    printf("verticalDiv = %d\n", verticalDiv);
    printf("coupling = %d\n", coupling);
    printf("verticalPosition = %d\n", coupling);
    printf("\n\n");
}

void IDSO1070A::Channel::setVerticalPosition(int i)
{
    if (i < 8)
    {
        i = 8;
    }
    else if (i > IDSO1070A_MAX_SAMPLE)
    {
        i = IDSO1070A_MAX_SAMPLE;
    }
    verticalPosition = i;
}

void IDSO1070A::Trigger::print()
{
    printf("[Trigger]\n");
    printf("isHold = %d\n", isHold);
    printf("mode = %d\n", mode);
    printf("channel = %d\n", channel);
    printf("slope = %d\n", slope);
    printf("level = %d\n", level);
    printf("xPosition = %lf\n", xPosition);
    printf("\n\n");
}

uint16_t IDSO1070A::Trigger::getBottomPWM()
{
    if (channel == TRIGCHAN_CH1)
        return innerTriggerPWM[0];
    else if (channel == TRIGCHAN_CH2)
        return innerTriggerPWM[2];
    return -1;
}

uint16_t IDSO1070A::Trigger::getTopPWM()
{
    if (channel == TRIGCHAN_CH1)
        return innerTriggerPWM[1];
    else if (channel == TRIGCHAN_CH2)
        return innerTriggerPWM[3];
    return -1;
}

void IDSO1070A::Trigger::setTriggerLevel(uint16_t i)
{
    if (i < 8)
    {
        i = 8;
    }
    else if (i > IDSO1070A_MAX_SAMPLE)
    {
        i = IDSO1070A_MAX_SAMPLE;
    }
    level = i;
}