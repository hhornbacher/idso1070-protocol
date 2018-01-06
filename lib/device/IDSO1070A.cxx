#include "IDSO1070A.h"

IDSO1070A::IDSO1070A()
{
    armFirmwareVersion[0] = 0;
    fpgaFirmwareVersion[0] = 0;
    memset(&eeromData, 0, sizeof(EEROMData));
    channel1.setVerticalPosition(188);
    channel2.setVerticalPosition(68);
    // this.channel1.setAttenuationFactor(AttenuationFactor.X1);
    // this.channel2.setAttenuationFactor(AttenuationFactor.X1);
}

void IDSO1070A::print()
{
    printf("[IDSO1070A]\n");
    printf("armFirmwareVersion = %s\n", armFirmwareVersion);
    printf("fpgaFirmwareVersion = %s\n", fpgaFirmwareVersion);
    printf("batteryLevel = %d\n", batteryLevel);
    printf("receiveFreqDivStatus = 0x%02x\n", receiveFreqDivStatus);
    printf("freqDiv = %d\n", freqDiv);
    printf("timeBase = %d\n", timeBase);
    printf("captureMode = %d\n", captureMode);
    printf("scopeMode = %d\n", scopeMode);
    channel1.print();
    channel2.print();
    trigger.print();
    eeromData.print();
    printf("\n\n");
}

TimeBase IDSO1070A::getTimeBase()
{
    return timeBase;
}
void IDSO1070A::setTimeBase(TimeBase timeBase)
{
    this->timeBase = timeBase;
}

CaptureMode IDSO1070A::getCaptureMode()
{
    return captureMode;
}
void IDSO1070A::setCaptureMode(CaptureMode captureMode)
{
    this->captureMode = captureMode;
}

ScopeMode IDSO1070A::getScopeMode()
{
    return scopeMode;
}
void IDSO1070A::setScopeMode(ScopeMode scopeMode)
{
    this->scopeMode = scopeMode;
}

EEROMData &IDSO1070A::getEEROMData()
{
    return eeromData;
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
    this->selectedChannel = &channel;
}

Trigger &IDSO1070A::getTrigger()
{
    return trigger;
}

void IDSO1070A::setBatteryLevel(uint8_t batteryLevel)
{
    this->batteryLevel = batteryLevel;
}
uint8_t IDSO1070A::getBatteryLevel()
{
    return batteryLevel;
}

void IDSO1070A::setLittlePacketStatus(int littlePacketStatus)
{
    this->littlePacketStatus = littlePacketStatus;
}
int IDSO1070A::getLittlePacketStatus()
{
    return littlePacketStatus;
}

void IDSO1070A::setReceiveFreqDivStatus(uint8_t receiveFreqDivStatus)
{
    this->receiveFreqDivStatus = receiveFreqDivStatus;
}
uint8_t IDSO1070A::getReceiveFreqDivStatus()
{
    return receiveFreqDivStatus;
}

void IDSO1070A::setFreqDiv(uint32_t freqDiv)
{
    this->freqDiv = freqDiv;
}
uint32_t IDSO1070A::getFreqDiv()
{
    return freqDiv;
}

void IDSO1070A::setARMFirmwareVersion(char *version)
{
    strncpy(armFirmwareVersion, version, 9);
}

void IDSO1070A::setFPGAFirmwareVersion(char *version)
{
    strncpy(fpgaFirmwareVersion, version, 9);
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
    return (MemoryDepth == 2000 && timeBase == HDIV_5uS) ? 2500 : MemoryDepth;
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
    return enabledChannelsCount == 0 ? 0 : (enabledChannelsCount * getSamplesNumberOfOneFrame()) / SamplesCountPerPacket;
}
