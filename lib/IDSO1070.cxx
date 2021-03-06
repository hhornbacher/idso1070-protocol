#include "IDSO1070.h"

IDSO1070::IDSO1070() : selectedChannel(CHANNEL_1), littlePacketStatus(0)
{
    setTimeBase(HDIV_10uS);
    setScopeMode(SCOMODE_ANALOG);
    setCaptureMode(CAPMODE_NORMAL);
    setBatteryLevel(0);

    enableChannel(CHANNEL_1);
    enableChannel(CHANNEL_2);
    setChannelVerticalDiv(CHANNEL_1, VDIV_1V);
    setChannelVerticalDiv(CHANNEL_2, VDIV_1V);
    setChannelCoupling(CHANNEL_1, COUPLING_AC);
    setChannelCoupling(CHANNEL_2, COUPLING_AC);
    setChannelVerticalPosition(CHANNEL_1, 136);
    setChannelVerticalPosition(CHANNEL_2, 120);

    setTriggerMode(TRIGMODE_AUTO);
    setTriggerChannel(TRIGCHAN_CH1);
    setTriggerSlope(TRIGSLOPE_RISING);
    setTriggerLevel(0);
    setTriggerXPosition(0.5);
}

IDSO1070 &IDSO1070::operator=(IDSO1070 obj)
{
    deviceSettings = obj.deviceSettings;
    channels[0] = obj.channels[0];
    channels[1] = obj.channels[1];
    triggerSettings = obj.triggerSettings;

    selectedChannel = obj.selectedChannel;
    littlePacketStatus = obj.littlePacketStatus;
    return *this;
}

TimeBase IDSO1070::getTimeBase()
{
    return deviceSettings.timeBase;
}
void IDSO1070::setTimeBase(TimeBase timeBase)
{
    deviceSettings.timeBase = timeBase;
}

CaptureMode IDSO1070::getCaptureMode()
{
    return deviceSettings.captureMode;
}
void IDSO1070::setCaptureMode(CaptureMode captureMode)
{
    deviceSettings.captureMode = captureMode;
}

ScopeMode IDSO1070::getScopeMode()
{
    return deviceSettings.scopeMode;
}
void IDSO1070::setScopeMode(ScopeMode scopeMode)
{
    deviceSettings.scopeMode = scopeMode;
}

void IDSO1070::setBatteryLevel(uint8_t batteryLevel)
{
    deviceSettings.batteryLevel = batteryLevel;
}
uint8_t IDSO1070::getBatteryLevel()
{
    return deviceSettings.batteryLevel;
}

bool IDSO1070::isSampling()
{
    return sampling;
}
void IDSO1070::setSampling(bool sampling)
{
    this->sampling = sampling;
}

void IDSO1070::setLittlePacketStatus(int littlePacketStatus)
{
    this->littlePacketStatus = littlePacketStatus;
}
int IDSO1070::getLittlePacketStatus()
{
    return littlePacketStatus;
}

void IDSO1070::setARMFirmwareVersion(string version)
{
    deviceSettings.armFirmwareVersion = version;
}

string IDSO1070::getARMFirmwareVersion()
{
    return deviceSettings.armFirmwareVersion;
}

void IDSO1070::setFPGAFirmwareVersion(string version)
{
    deviceSettings.fpgaFirmwareVersion = version;
}

string IDSO1070::getFPGAFirmwareVersion()
{
    return deviceSettings.fpgaFirmwareVersion;
}

void IDSO1070::setProductName(string productName)
{
    deviceSettings.productName = productName;
}

string IDSO1070::getProductName()
{
    return deviceSettings.productName;
}

void IDSO1070::setUserName(string userName)
{
    deviceSettings.userName = userName;
}

string IDSO1070::getUserName()
{
    return deviceSettings.userName;
}

void IDSO1070::setCaliLevel(uint8_t *data)
{
    memcpy(deviceSettings.caliLevel, data, sizeof(deviceSettings.caliLevel));
}

uint16_t *IDSO1070::getCaliLevel()
{
    return deviceSettings.caliLevel;
}

void IDSO1070::setFPGAAlert(uint8_t *data)
{
    memcpy(deviceSettings.fpgaAlert, data, sizeof(deviceSettings.fpgaAlert));
}

uint8_t *IDSO1070::getFPGAAlert()
{
    return deviceSettings.fpgaAlert;
}

void IDSO1070::setDiffFixData(size_t channel, size_t offset, uint8_t *data)
{
    if (
        (offset != 0 && offset != 100 && offset != 200) ||
        channel > 1)
        return;

    if (offset < 200)
    {
        memcpy(&deviceSettings.diffFixData[channel][offset], data, 100);
    }
    else
        memcpy(&deviceSettings.diffFixData[channel][offset], data, 56);
}

size_t IDSO1070::getSamplesNumberOfOneFrame()
{
    return (deviceSettings.timeBase == HDIV_5uS) ? 2500 : MemoryDepth;
}
uint8_t IDSO1070::getEnabledChannelsCount()
{
    if (isChannelEnabled(CHANNEL_1) && isChannelEnabled(CHANNEL_2))
        return 2;
    else if (isChannelEnabled(CHANNEL_1) || isChannelEnabled(CHANNEL_2))
        return 1;
    return 0;
}
uint8_t IDSO1070::getPacketsNumber()
{
    return (getEnabledChannelsCount() * getSamplesNumberOfOneFrame()) / SamplesCountPerPacket;
}

// Trigger methods

uint16_t IDSO1070::getTriggerBottomPWM()
{
    if (triggerSettings.channel == TRIGCHAN_CH1)
        return triggerSettings.innerPWM[0];
    else if (triggerSettings.channel == TRIGCHAN_CH2)
        return triggerSettings.innerPWM[2];
    return -1;
}

uint16_t IDSO1070::getTriggerTopPWM()
{
    if (triggerSettings.channel == TRIGCHAN_CH1)
        return triggerSettings.innerPWM[1];
    else if (triggerSettings.channel == TRIGCHAN_CH2)
        return triggerSettings.innerPWM[3];
    return -1;
}

void IDSO1070::setTriggerLevel(uint16_t i)
{
    if (i < 8)
    {
        i = 8;
    }
    else if (i > MaxSample)
    {
        i = MaxSample;
    }
    triggerSettings.level = i;
}
uint16_t IDSO1070::getTriggerLevel()
{
    return triggerSettings.level;
}

void IDSO1070::setTriggerChannel(TriggerChannel channel)
{
    triggerSettings.channel = channel;
}
TriggerChannel IDSO1070::getTriggerChannel()
{
    return triggerSettings.channel;
}

void IDSO1070::setTriggerSlope(TriggerSlope slope)
{
    triggerSettings.slope = slope;
}
TriggerSlope IDSO1070::getTriggerSlope()
{
    return triggerSettings.slope;
}

void IDSO1070::setTriggerMode(TriggerMode mode)
{
    triggerSettings.mode = mode;
}
TriggerMode IDSO1070::getTriggerMode()
{
    return triggerSettings.mode;
}

double IDSO1070::getTriggerXPosition()
{
    return triggerSettings.xPosition;
}
void IDSO1070::setTriggerXPosition(double xPosition)
{
    triggerSettings.xPosition = xPosition;
}

void IDSO1070::setTriggerInnerPWM(uint8_t index, uint16_t pwm)
{
    triggerSettings.innerPWM[index] = pwm;
}
uint16_t IDSO1070::getTriggerInnerPWM(uint8_t index)
{
    return triggerSettings.innerPWM[index];
}
uint16_t *IDSO1070::getTriggerInnerPWM()
{
    return triggerSettings.innerPWM;
}

void IDSO1070::setTriggerOuterPWM(uint8_t index, uint16_t pwm)
{
    triggerSettings.outerPWM[index] = pwm;
}
uint16_t IDSO1070::getTriggerOuterPWM(uint8_t index)
{
    return triggerSettings.outerPWM[index];
}
uint16_t *IDSO1070::getTriggerOuterPWM()
{
    return triggerSettings.outerPWM;
}

// Channel methods
void IDSO1070::setSelectedChannel(ChannelSelector channel)
{
    this->selectedChannel = channel;
}

ChannelSelector IDSO1070::getSelectedChannel()
{
    return selectedChannel;
}

void IDSO1070::enableChannel(ChannelSelector channel)
{
    channels[channel].enabled = true;
}

void IDSO1070::disableChannel(ChannelSelector channel)
{
    channels[channel].enabled = false;
}

bool IDSO1070::isChannelEnabled(ChannelSelector channel)
{
    return channels[channel].enabled;
}

void IDSO1070::setChannelVerticalDiv(ChannelSelector channel, VoltageDiv verticalDiv)
{
    channels[channel].verticalDiv = verticalDiv;
}

VoltageDiv IDSO1070::getChannelVerticalDiv(ChannelSelector channel)
{
    return channels[channel].verticalDiv;
}

void IDSO1070::setChannelCoupling(ChannelSelector channel, InputCoupling coupling)
{
    channels[channel].coupling = coupling;
}

InputCoupling IDSO1070::getChannelCoupling(ChannelSelector channel)
{
    return channels[channel].coupling;
}

void IDSO1070::setChannelVerticalPosition(ChannelSelector channel, int16_t verticalPosition)
{
    if (verticalPosition < 8)
        verticalPosition = 8;
    else if (verticalPosition > MaxSample)
        verticalPosition = MaxSample;
    channels[channel].verticalPosition = verticalPosition;
}

int8_t IDSO1070::getChannelVerticalPosition(ChannelSelector channel)
{
    return channels[channel].verticalPosition;
}

void IDSO1070::setChannelPWM(ChannelSelector channel, uint16_t pwm, uint8_t a, uint8_t b)
{
    channels[channel].pwmArray[a][b] = pwm;
}

uint16_t IDSO1070::getChannelPWM(ChannelSelector channel, uint8_t a, uint8_t b)
{
    if (a < 9 && b < 2)
        return channels[channel].pwmArray[a][b];
    return -1;
}

uint16_t *IDSO1070::getChannelPWM(ChannelSelector channel, uint8_t a)
{
    if (a < 9)
        return channels[channel].pwmArray[a];
    return NULL;
}

void IDSO1070::setChannelVoltage125(ChannelSelector channel, double voltage)
{
    channels[channel].voltage125 = voltage;
}

double IDSO1070::getChannelVoltage125(ChannelSelector channel)
{
    return channels[channel].voltage125;
}

void IDSO1070::setChannelVoltageRL1(ChannelSelector channel, double voltage)
{
    channels[channel].voltageRL1 = voltage;
}

double IDSO1070::getChannelVoltageRL1(ChannelSelector channel)
{
    return channels[channel].voltageRL1;
}

void IDSO1070::setChannelVoltageRL2(ChannelSelector channel, double voltage)
{
    channels[channel].voltageRL2 = voltage;
}

double IDSO1070::getChannelVoltageRL2(ChannelSelector channel)
{
    return channels[channel].voltageRL2;
}
