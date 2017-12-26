#include "IDSO1070A.h"

bool IDSO1070A::isSampleRate200Mor250M()
{
    return timeBase <= HDIV_1uS;
}

size_t IDSO1070A::getSamplesNumberOfOneFrame()
{
    return (memoryDepth == 2000 && timeBase == HDIV_5uS) ? 2500 : memoryDepth;
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
    return enabledChannelsCount == 0 ? 0 : (enabledChannelsCount * getSamplesNumberOfOneFrame()) / samplesCountPerPacket;
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

void IDSO1070A::Channel::print()
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

void IDSO1070A::Trigger::print()
{
    printf(
        "[Trigger]\n"
        "\n\n");
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