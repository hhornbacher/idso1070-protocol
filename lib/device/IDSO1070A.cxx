#include "IDSO1070A.h"

void IDSO1070A::print()
{
    printf("[IDSO1070A]\n");
    channel1.print();
    channel2.print();
    trigger.print();
    printf("batteryLevel = %d\n", batteryLevel);
    printf("receiveFreqDivStatus = 0x%02x\n", receiveFreqDivStatus);
    printf("freqDiv = %d\n", freqDiv);
    printf("timeBase = %d\n", timeBase);
    printf("captureMode = %d\n", captureMode);
    printf("scopeMode = %d\n", scopeMode);
    printf("\n\n");
}

TimeBase IDSO1070A::getTimeBase()
{
    return timeBase;
}
void IDSO1070A::setTimeBase()
{
}

CaptureMode IDSO1070A::getCaptureMode()
{
    return captureMode;
}
void IDSO1070A::setCaptureMode(CaptureMode captureMode)
{
}

ScopeMode IDSO1070A::getScopeMode()
{
    return scopeMode;
}
void IDSO1070A::setScopeMode(ScopeMode scopeMode)
{
}

Channel &IDSO1070A::getChannel1()
{
    return channel1;
}
Channel &IDSO1070A::getChannel2()
{
    return channel2;
}

Channel &IDSO1070A::getSelectedChannel()
{
    return *selectedChannel;
}

void IDSO1070A::setSelectedChannel(Channel &channel)
{
}

Trigger &IDSO1070A::getTrigger()
{
    return trigger;
}

void IDSO1070A::setBatteryLevel(uint8_t batteryLevel)
{
}
uint8_t IDSO1070A::getBatteryLevel()
{
    return batteryLevel;
}

void IDSO1070A::setReceiveFreqDivStatus(uint8_t receiveFreqDivStatus)
{
}
uint8_t IDSO1070A::getReceiveFreqDivStatus()
{
    return receiveFreqDivStatus;
}

void IDSO1070A::setFreqDiv(uint32_t freqDiv)
{
}
uint32_t IDSO1070A::getFreqDiv()
{
    return freqDiv;
}

bool IDSO1070A::isSampleRate200Mor250M()
{
    return timeBase <= HDIV_1uS;
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

size_t IDSO1070A::getSamplesNumberOfOneFrame()
{
    return (IDSO1070A_MEMORY_DEPTH == 2000 && timeBase == HDIV_5uS) ? 2500 : IDSO1070A_MEMORY_DEPTH;
}
uint8_t IDSO1070A::getEnabledChannelsCount()
{
    if (channel1.isEnabled() && channel2.isEnabled())
        return 2;
    else if (channel1.isEnabled() || channel2.isEnabled())
        return 1;
    return 0;
}
uint8_t IDSO1070A::getPacketsNumber()
{
    int enabledChannelsCount = getEnabledChannelsCount();
    return enabledChannelsCount == 0 ? 0 : (enabledChannelsCount * getSamplesNumberOfOneFrame()) / IDSO1070A_SAMPLES_COUNT_PER_PACKET;
}
