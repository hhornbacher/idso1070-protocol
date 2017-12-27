#include "CommandGenerator.h"

float CommandGenerator::mapValue(int i, float f, float f2, float f3, float f4)
{
    return (((((float)i) - f) / (f2 - f)) * (f4 - f3)) + f3;
}

float CommandGenerator::mapValue(float f, float f2, float f3, float f4, float f5)
{
    return (((f - f2) / (f3 - f2)) * (f5 - f4)) + f4;
}

CommandQueue CommandGenerator::readFPGAVersionAndEEROM(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(readFPGAVersion(device));
    cmds.add(readEEROMPage(0x00));
    // cmds.push_back(readEEROMPage(0x01));
    cmds.add(readEEROMPage(0x04));
    cmds.add(readEEROMPage(0x05));
    // cmds.push_back(readEEROMPage(0x06));
    cmds.add(readEEROMPage(0x07));
    cmds.add(readEEROMPage(0x08));
    cmds.add(readEEROMPage(0x09));
    cmds.add(readEEROMPage(0x0a));
    cmds.add(readEEROMPage(0x0b));
    cmds.add(readEEROMPage(0x0c));
    // cmds.push_back(readEEROMPage(0x0f));
    return cmds;
}

CommandQueue CommandGenerator::updateTimeBase(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(updateSampleRate(device));
    cmds.add(getFreqDivLowBytes(device));
    cmds.add(getFreqDivHighBytes(device));
    // arrayList.addAll(getXTriggerPosCommands(idso1070));
    cmds.add(readRamCount(device));
    return cmds;
}

CommandQueue CommandGenerator::initialize(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(channelStatusOnly(device));
    cmds.add(voltageDiv(device));
    cmds.add(updateTimeBase(device));
    cmds.add(trigger(device));
    cmds.add(channel1Level(device));
    cmds.add(channel2Level(device));
    cmds.add(channel1Coupling(device));
    cmds.add(channel2Coupling(device));
    cmds.add(pullSamples(device));
    return cmds;
}

CommandQueue CommandGenerator::voltageDiv(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(updateChannelVolts125(device));
    cmds.add(relay1(device));
    cmds.add(relay2(device));
    cmds.add(relay3(device));
    cmds.add(relay4(device));
    cmds.add(channel1Level(device));
    cmds.add(channel2Level(device));
    return cmds;
}

CommandQueue CommandGenerator::pullSamples(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(updateTriggerMode(device));
    uint8_t cmd[4] = {0xaa, 0x04, 0x00, 0x00};
    cmds.add(new Command(cmd));
    return cmds;
}

CommandQueue CommandGenerator::channelStatus(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(channelStatusOnly(device));
    cmds.add(updateTimeBase(device));
    return cmds;
}

CommandQueue CommandGenerator::channelStatusOnly(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(selectChannel(device));
    cmds.add(selectRAMChannel(device));
    cmds.add(readRamCount(device));
    return cmds;
}

CommandQueue CommandGenerator::updateTriggerSource(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(updateTriggerSourceAndSlope(device));
    cmds.add(updateTriggerLevel(device));
    return cmds;
}

CommandQueue CommandGenerator::levels(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(channel1Level(device));
    cmds.add(channel2Level(device));
    cmds.add(updateTriggerLevel(device));
    return cmds;
}

CommandQueue CommandGenerator::trigger(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(updateTriggerSourceAndSlope(device));
    cmds.add(updateTriggerMode(device));
    cmds.add(updateTriggerLevel(device));
    return cmds;
}

CommandQueue CommandGenerator::xTriggerPos(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(preTrigger(device));
    cmds.add(postTrigger(device));
    return cmds;
}

CommandQueue CommandGenerator::channel1VoltageDiv(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(updateChannelVolts125(device));
    cmds.add(relay1(device));
    cmds.add(relay2(device));
    cmds.add(channel1Level(device));
    return cmds;
}

CommandQueue CommandGenerator::channel2VoltageDiv(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(updateChannelVolts125(device));
    cmds.add(relay3(device));
    cmds.add(relay4(device));
    cmds.add(channel2Level(device));
    return cmds;
}

Command *CommandGenerator::readEEROMPage(uint8_t address)
{
    uint8_t cmdBuffer[4] = {0xee, 0xaa, address, 0x00};
    return new Command(cmdBuffer, 2);
}

Command *CommandGenerator::readFPGAVersion(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {0xaa, 0x02, 0x00, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::readBatteryLevel(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {0x57, 0x03, 0x00, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::updateSampleRate(IDSO1070A &device)
{
    uint8_t b = 0;

    switch (device.timeBase)
    {
    case HDIV_5nS:
    case HDIV_10nS:
    case HDIV_20nS:
    case HDIV_50nS:
    case HDIV_100nS:
    case HDIV_200nS:
        b = 0x01;
        break;
    case HDIV_500nS:
    case HDIV_1uS:
    case HDIV_2uS:
    case HDIV_5uS:
    case HDIV_10uS:
    case HDIV_20uS:
    case HDIV_50uS:
    case HDIV_100uS:
    case HDIV_200uS:
    case HDIV_500uS:
    case HDIV_1mS:
    case HDIV_2mS:
    case HDIV_5mS:
    case HDIV_10mS:
    case HDIV_20mS:
    case HDIV_50mS:
    case HDIV_100mS:
    case HDIV_200mS:
    case HDIV_500mS:
    case HDIV_1S:
    case HDIV_2S:
    case HDIV_5S:
    case HDIV_10S:
    case HDIV_20S:
    case HDIV_50S:
    case HDIV_100S:
    case HDIV_200S:
    case HDIV_500S:
        b = 0x00;
        break;
    }
    if (!device.channel1.enabled || !device.channel2.enabled)
    {
        switch (device.timeBase)
        {
        case HDIV_5nS:
        case HDIV_10nS:
        case HDIV_20nS:
        case HDIV_50nS:
        case HDIV_100nS:
        case HDIV_200nS:
        case HDIV_500nS:
        case HDIV_1uS:
            b |= 0x02;
            break;
        case HDIV_2uS:
        case HDIV_5uS:
        case HDIV_10uS:
        case HDIV_20uS:
        case HDIV_50uS:
        case HDIV_100uS:
        case HDIV_200uS:
        case HDIV_500uS:
        case HDIV_1mS:
        case HDIV_2mS:
        case HDIV_5mS:
        case HDIV_10mS:
        case HDIV_20mS:
        case HDIV_50mS:
        case HDIV_100mS:
        case HDIV_200mS:
        case HDIV_500mS:
        case HDIV_1S:
        case HDIV_2S:
        case HDIV_5S:
        case HDIV_10S:
        case HDIV_20S:
        case HDIV_50S:
        case HDIV_100S:
        case HDIV_200S:
        case HDIV_500S:
            b &= ~0x02;
            break;
        default:
            break;
        }
    }
    b &= ~0x02;
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)CMD_SAMPLE_RATE, b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::getFreqDivLowBytes(IDSO1070A &device)
{
    // newTimeBase = idso1070.timeBase;
    uint16_t freqDiv = (uint16_t)(device.getTimebaseFromFreqDiv() & 0xffff);
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_FREQ_DIV_LOW,
        (uint8_t)(freqDiv & 0xff),
        (uint8_t)((freqDiv >> 8) & 0xff)};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::getFreqDivHighBytes(IDSO1070A &device)
{
    uint16_t freqDiv = (uint16_t)((device.getTimebaseFromFreqDiv() >> 16) & 0xffff);
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_FREQ_DIV_HIGH,
        (uint8_t)(freqDiv & 0xff),
        (uint8_t)((freqDiv >> 8) & 0xff)};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::selectRAMChannel(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_CHANNEL_SELECTION,
        0x00, 0x00};
    if (device.channel1.enabled && !device.channel2.enabled)
    {
        cmdBuffer[2] = 0x08;
    }
    else if (device.channel2.enabled && !device.channel1.enabled)
    {
        cmdBuffer[2] = 0x09;
    }
    else if (device.channel2.enabled && device.channel1.enabled)
    {
        cmdBuffer[2] = 0x00;
    }
    else
    {
        cmdBuffer[2] = 0x01;
    }
    return new Command(cmdBuffer);
}

Command *CommandGenerator::updateChannelVolts125(IDSO1070A &device)
{
    uint8_t b = 0;
    switch (device.channel1.verticalDiv)
    {
    case VDIV_10mV:
    case VDIV_100mV:
    case VDIV_1V:
        b &= ~0x03;
        break;
    case VDIV_20mV:
    case VDIV_200mV:
    case VDIV_2V:
        b &= ~0x02;
        b |= 0x01;
        break;
    case VDIV_50mV:
    case VDIV_500mV:
    case VDIV_5V:
        b &= ~0x01;
        b |= 0x02;
        break;
    }

    switch (device.channel2.verticalDiv)
    {
    case VDIV_10mV:
    case VDIV_100mV:
    case VDIV_1V:
        b &= ~0x0c;
        break;
    case VDIV_20mV:
    case VDIV_200mV:
    case VDIV_2V:
        b &= ~0x08;
        b |= 0x04;
        break;
    case VDIV_50mV:
    case VDIV_500mV:
    case VDIV_5V:
        b &= ~0x04;
        b |= 0x08;
        break;
    }
    b &= ~0x30;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_CHANNEL_VOLTS_DIV_125,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::relay1(IDSO1070A &device)
{
    uint8_t b = 0;

    switch (device.channel1.verticalDiv)
    {
    case VDIV_10mV:
    case VDIV_100mV:
    case VDIV_20mV:
    case VDIV_200mV:
    case VDIV_50mV:
    case VDIV_500mV:
        b = 0x7f;
        break;
    case VDIV_1V:
    case VDIV_2V:
    case VDIV_5V:
        b = 0x80;
        break;
    }

    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_SET_RELAY,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::relay2(IDSO1070A &device)
{
    uint8_t b = 0;

    switch (device.channel1.verticalDiv)
    {
    case VDIV_10mV:
    case VDIV_100mV:
    case VDIV_20mV:
    case VDIV_200mV:
    case VDIV_50mV:
    case VDIV_500mV:
        b = 0xfd;
        break;
    case VDIV_1V:
    case VDIV_2V:
    case VDIV_5V:
        b = 0x02;
        break;
    }

    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_SET_RELAY,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::relay3(IDSO1070A &device)
{
    uint8_t b = 0;

    switch (device.channel1.verticalDiv)
    {
    case VDIV_10mV:
    case VDIV_100mV:
    case VDIV_20mV:
    case VDIV_200mV:
    case VDIV_50mV:
    case VDIV_500mV:
        b = 0xbf;
        break;
    case VDIV_1V:
    case VDIV_2V:
    case VDIV_5V:
        b = 0x40;
        break;
    }

    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_SET_RELAY,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::relay4(IDSO1070A &device)
{
    uint8_t b = 0;

    switch (device.channel1.verticalDiv)
    {
    case VDIV_10mV:
    case VDIV_100mV:
    case VDIV_20mV:
    case VDIV_200mV:
    case VDIV_50mV:
    case VDIV_500mV:
        b = 0xfb;
        break;
    case VDIV_1V:
    case VDIV_2V:
    case VDIV_5V:
        b = 0x04;
        break;
    }

    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_SET_RELAY,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::channel1Coupling(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_SET_RELAY,
        0x00, 0x00};
    if (device.channel1.coupling == COUPLING_GND)
    {
        cmdBuffer[2] = 0xff;
        cmdBuffer[3] = 0x01;
    }
    else if (device.channel1.coupling == COUPLING_DC)
    {
        cmdBuffer[2] = 0xef;
    }
    else
    {
        cmdBuffer[2] = 0x10;
    }
    return new Command(cmdBuffer);
}

Command *CommandGenerator::channel2Coupling(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_SET_RELAY,
        0x00, 0x00};
    if (device.channel2.coupling == COUPLING_GND)
    {
        cmdBuffer[2] = 0xff;
        cmdBuffer[3] = 0x02;
    }
    else if (device.channel2.coupling == COUPLING_DC)
    {
        cmdBuffer[2] = 0xfe;
    }
    else
    {
        cmdBuffer[2] = 0x01;
    }
    return new Command(cmdBuffer);
}

Command *CommandGenerator::updateTriggerMode(IDSO1070A &device)
{
    uint8_t b = 0;
    if (device.captureMode == CAPMODE_ROLL)
        b = (1 << 0);
    if (device.trigger.mode == TRIGMODE_AUTO)
        b |= (1 << 1);
    else if (device.trigger.mode == TRIGMODE_SINGLE)
        b |= (1 << 2);
    else if (device.captureMode != CAPMODE_NORMAL)
        b |= (1 << 3);
    else if (device.scopeMode == SCOMODE_DIGITAL)
        b |= (1 << 4);
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_TRIGGER_MODE,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::readRamCount(IDSO1070A &device)
{
    uint16_t b = 0;
    if (device.channel1.enabled && device.channel2.enabled)
    {
        b = (uint16_t)device.getSamplesNumberOfOneFrame();
    }
    else if (device.channel1.enabled || device.channel2.enabled)
    {
        if (!device.isSampleRate200Mor250M())
            b = (uint16_t)(device.getSamplesNumberOfOneFrame() / 2);
        else
        {
            double x = ((double)device.getSamplesNumberOfOneFrame() * device.trigger.xPosition / 2) + ((double)device.getSamplesNumberOfOneFrame() * (1 - device.trigger.xPosition));
            b = (uint16_t)x;
        }
    }
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_READ_RAM_COUNT,
        (uint8_t)(b & 0xff),
        (uint8_t)((b >> 8) & 0xf + ((device.getPacketsNumber() - 1) << 4))};
    return new Command(cmdBuffer);
    return NULL;
}

Command *CommandGenerator::channel1Level(IDSO1070A &device)
{
    int verticalDiv = (int)device.channel1.verticalDiv;
    return channel1PWM(
        device,
        (uint16_t)mapValue(
            device.channel1.verticalPosition,
            8.0f, 248.0f,
            (float)device.pwmArray[verticalDiv][0], (float)device.pwmArray[verticalDiv][1]));
}
Command *CommandGenerator::channel2Level(IDSO1070A &device)
{
    int verticalDiv = (int)device.channel2.verticalDiv;
    return channel1PWM(
        device,
        (uint16_t)mapValue(
            device.channel2.verticalPosition,
            8.0f, 248.0f,
            (float)device.pwmArray[verticalDiv][0], (float)device.pwmArray[verticalDiv][1]));
}

Command *CommandGenerator::updateTriggerSourceAndSlope(IDSO1070A &device)
{
    uint8_t b =
        device.trigger.channel == TRIGCHAN_CH1 ? 0x01 : device.trigger.channel == TRIGCHAN_CH2 ? 0x00 : device.trigger.channel == TRIGCHAN_EXT ? 0x02 : 0x03;

    b &= ~0x2c;
    if (device.scopeMode == SCOMODE_ANALOG)
        b |= 0x10;
    else
        b &= ~0x10;
    if (device.trigger.slope == TRIGSLOPE_RISING)
        b |= 0x80;
    else
        b &= ~0x80;

    uint8_t cmdBuffer[4] = {0x55, (uint8_t)CMD_TRIGGER_SOURCE, b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::updateTriggerLevel(IDSO1070A &device)
{
    uint16_t pwm = mapValue(device.trigger.level, 8.0f, 248.0f, (float)device.trigger.getBottomPWM(), (float)device.trigger.getTopPWM());
    return updateTriggerPWM(device, pwm);
}

Command *CommandGenerator::updateTriggerPWM(IDSO1070A &device, uint16_t pwm)
{
    if (pwm<0 | pwm> device.maxPWM)
        return NULL;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_TRIGGER_PWM,
        (uint8_t)(pwm & 0xff),
        (uint8_t)((pwm >> 8) & 0x0f)};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::selectChannel(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_CHANNEL_SELECTION,
        0x00, 0x00};
    if (device.channel1.enabled && !device.channel2.enabled && device.isSampleRate200Mor250M())
    {
        cmdBuffer[2] = 0x00;
    }
    else if (device.channel2.enabled && !device.channel1.enabled && device.isSampleRate200Mor250M())
    {
        cmdBuffer[2] = 0x01;
    }
    else if (device.getEnabledChannelsCount() == 2 || (!device.isSampleRate200Mor250M() && device.getEnabledChannelsCount() == 1))
    {
        cmdBuffer[2] = 0x02;
    }
    else
    {
        cmdBuffer[2] = 0x03;
    }
    return new Command(cmdBuffer);
}

Command *CommandGenerator::preTrigger(IDSO1070A &device)
{
    uint16_t i = ((uint16_t)(((double)device.getSamplesNumberOfOneFrame()) * device.trigger.xPosition)) + 5;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_PRE_TRIGGER_LENGTH,
        (uint8_t)(i & 0xff),
        (uint8_t)((i >> 8) & 0xff)};
    return new Command(cmdBuffer);
}
Command *CommandGenerator::postTrigger(IDSO1070A &device)
{
    uint16_t i = ((uint16_t)(((double)device.getSamplesNumberOfOneFrame()) * (1 - device.trigger.xPosition)));
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_POST_TRIGGER_LENGTH,
        (uint8_t)(i & 0xff),
        (uint8_t)((i >> 8) & 0xff)};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::channel1PWM(IDSO1070A &device, uint16_t pwm)
{
    if (pwm < 0 || pwm > device.maxPWM)
        return NULL;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_CH1_PWM,
        (uint8_t)(pwm & 0xff),
        (uint8_t)((pwm >> 8) & 0x0f)};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::channel2PWM(IDSO1070A &device, uint16_t pwm)
{
    if (pwm < 0 || pwm > device.maxPWM)
        return NULL;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CMD_CH2_PWM,
        (uint8_t)(pwm & 0xff),
        (uint8_t)((pwm >> 8) & 0x0f)};
    return new Command(cmdBuffer);
}