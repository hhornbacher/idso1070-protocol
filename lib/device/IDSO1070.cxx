#include "device/IDSO1070.h"

IDSO1070::IDSO1070()
{
    armFirmwareVersion[0] = 0;
    fpgaFirmwareVersion[0] = 0;
    memset(&eeromData, 0, sizeof(EEROMData));
    channel1.setVerticalPosition(188);
    channel2.setVerticalPosition(68);
    // this.channel1.setAttenuationFactor(AttenuationFactor.X1);
    // this.channel2.setAttenuationFactor(AttenuationFactor.X1);
}

TimeBase IDSO1070::getTimeBase()
{
    return timeBase;
}
void IDSO1070::setTimeBase(TimeBase timeBase)
{
    this->timeBase = timeBase;
}

CaptureMode IDSO1070::getCaptureMode()
{
    return captureMode;
}
void IDSO1070::setCaptureMode(CaptureMode captureMode)
{
    this->captureMode = captureMode;
}

ScopeMode IDSO1070::getScopeMode()
{
    return scopeMode;
}
void IDSO1070::setScopeMode(ScopeMode scopeMode)
{
    this->scopeMode = scopeMode;
}

EEROMData &IDSO1070::getEEROMData()
{
    return eeromData;
}

Channel &IDSO1070::getChannel1()
{
    return channel1;
}
Channel &IDSO1070::getChannel2()
{
    return channel2;
}

Channel &IDSO1070::getSelectedChannel()
{
    return *selectedChannel;
}

int IDSO1070::getSelectedChannelIndex()
{
    if (selectedChannel == &channel1)
        return 1;
    else if (selectedChannel == &channel2)
        return 2;
    else
        return -1;
}

void IDSO1070::setSelectedChannel(Channel &channel)
{
    this->selectedChannel = &channel;
}

Trigger &IDSO1070::getTrigger()
{
    return trigger;
}

void IDSO1070::setBatteryLevel(uint8_t batteryLevel)
{
    this->batteryLevel = batteryLevel;
}
uint8_t IDSO1070::getBatteryLevel()
{
    return batteryLevel;
}

void IDSO1070::setLittlePacketStatus(int littlePacketStatus)
{
    this->littlePacketStatus = littlePacketStatus;
}
int IDSO1070::getLittlePacketStatus()
{
    return littlePacketStatus;
}

void IDSO1070::setReceiveFreqDivStatus(uint8_t receiveFreqDivStatus)
{
    this->receiveFreqDivStatus = receiveFreqDivStatus;
}
uint8_t IDSO1070::getReceiveFreqDivStatus()
{
    return receiveFreqDivStatus;
}

void IDSO1070::setFreqDiv(uint32_t freqDiv)
{
    this->freqDiv = freqDiv;
}
uint32_t IDSO1070::getFreqDiv()
{
    return freqDiv;
}

void IDSO1070::setARMFirmwareVersion(char *version)
{
    strncpy(armFirmwareVersion, version, 9);
}

void IDSO1070::setFPGAFirmwareVersion(char *version)
{
    strncpy(fpgaFirmwareVersion, version, 9);
}

bool IDSO1070::isSampleRate200Mor250M()
{
    return timeBase <= HDIV_1uS;
}

TimeBase IDSO1070::getTimebaseFromFreqDiv()
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

size_t IDSO1070::getSamplesNumberOfOneFrame()
{
    return (timeBase == HDIV_5uS) ? 2500 : MemoryDepth;
}
uint8_t IDSO1070::getEnabledChannelsCount()
{
    if (channel1.isEnabled() && channel2.isEnabled())
        return 2;
    else if (channel1.isEnabled() || channel2.isEnabled())
        return 1;
    return 0;
}
uint8_t IDSO1070::getPacketsNumber()
{
    return (getEnabledChannelsCount() * getSamplesNumberOfOneFrame()) / SamplesCountPerPacket;
}
