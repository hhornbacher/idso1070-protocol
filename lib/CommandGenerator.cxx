#include "CommandGenerator.h"

Command *CommandGenerator::readEEROMPage(uint8_t address)
{
    uint8_t cmdBuffer[4] = {0xee, 0xaa, address, 0x00};
    return new Command(cmdBuffer, 2);
}

CommandQueue CommandGenerator::readEEROM(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.push_back(readFPGAVersion(device));
    cmds.push_back(readEEROMPage(0x00));
    // cmds.push_back(readEEROMPage(0x01));
    cmds.push_back(readEEROMPage(0x04));
    cmds.push_back(readEEROMPage(0x05));
    // cmds.push_back(readEEROMPage(0x06));
    cmds.push_back(readEEROMPage(0x07));
    cmds.push_back(readEEROMPage(0x08));
    cmds.push_back(readEEROMPage(0x09));
    cmds.push_back(readEEROMPage(0x0a));
    cmds.push_back(readEEROMPage(0x0b));
    cmds.push_back(readEEROMPage(0x0c));
    // cmds.push_back(readEEROMPage(0x0f));
    return cmds;
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
Command *CommandGenerator::getSampleRate(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)SAMPLE_RATE, 0x00, 0x00};
    return new Command(cmdBuffer);
}
Command *CommandGenerator::getFreqDivLowBytes(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)FREQ_DIV_LOW, 0x00, 0x00};
    return new Command(cmdBuffer);
}
Command *CommandGenerator::getFreqDivHighBytes(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)FREQ_DIV_HIGH, 0x04, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::getTriggerSourceAndSlope(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)TRIGGER_SOURCE, 0x91, 0x00};

    return new Command(cmdBuffer);
    // CommandPacket commandPacket = new CommandPacket(Commands.TRIGGER_SOURCE);
    // byte unsetByteBit = idso1070.getTrigger().getTriggerChannel() == TriggerChannel.CH1 ? Utility.unsetByteBit(Utility.setByteBit((byte) 0, 0), 1) : idso1070.getTrigger().getTriggerChannel() == TriggerChannel.CH2 ? Utility.unsetByteBit(Utility.unsetByteBit((byte) 0, 0), 1) : idso1070.getTrigger().getTriggerChannel() == TriggerChannel.EXT ? Utility.setByteBit(Utility.unsetByteBit((byte) 0, 0), 1) : Utility.setByteBit(Utility.setByteBit((byte) 0, 0), 1);
    // unsetByteBit = Utility.unsetByteBit(Utility.unsetByteBit(unsetByteBit, 2), 3);
    // unsetByteBit = Utility.unsetByteBit(idso1070.getScopeMode() == ScopeMode.ANALOG ? Utility.setByteBit(unsetByteBit, 4) : Utility.unsetByteBit(unsetByteBit, 4), 5);
    // commandPacket.setData(2, idso1070.getTrigger().getTriggerSlope() == TriggerSlope.RISING_EDGE ? Utility.setByteBit(unsetByteBit, 7) : Utility.unsetByteBit(unsetByteBit, 7));
    // commandPacket.setData(3, 0);
    // return commandPacket;
}

Command *CommandGenerator::getTriggerLevel(IDSO1070A &device)
{
    return getTriggerPwm(0x91);
    // return getTriggerPwm((int) Utility.Map(idso1070.trigger.getTriggerLevel(), 8.0f, 248.0f, (float) idso1070.trigger.getBottomPwm(), (float) idso1070.trigger.getTopPwm()));
}

Command *CommandGenerator::getTriggerPwm(int i)
{
    if (i < 0)
        return NULL;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)TRIGGER_PWM,
        (uint8_t)(i & 0xff),
        (uint8_t)((i >> 8) & 0x0f)};
    return new Command(cmdBuffer);
}

CommandQueue CommandGenerator::getTriggerSource(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.push_back(getTriggerSourceAndSlope(device));
    cmds.push_back(getTriggerLevel(device));
    return cmds;
}

Command *CommandGenerator::selectChannel(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CHANNEL_SELECTION,
        0x00, 0x00};
    if (device.channel1.enabled && !device.channel2.enabled /*&& sampRate200Mor250M*/)
    {
        cmdBuffer[2] = 0x00;
    }
    else if (device.channel2.enabled && !device.channel1.enabled /*&& sampRate200Mor250M*/)
    {
        cmdBuffer[2] = 0x01;
    }
    else if ((device.channel2.enabled && device.channel1.enabled) /*|| (!sampRate200Mor250M && ((channel1 && !channel2) || (channel2 && !channel1)))*/)
    {
        cmdBuffer[2] = 0x02;
    }
    else
    {
        cmdBuffer[2] = 0x03;
    }
    return new Command(cmdBuffer);
}

CommandQueue CommandGenerator::getTimebase(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.push_back(getSampleRate(device));
    cmds.push_back(getFreqDivLowBytes(device));
    cmds.push_back(getFreqDivHighBytes(device));
    return cmds;
}

CommandQueue CommandGenerator::initialize(IDSO1070A &device)
{
    CommandQueue cmds;
    CommandQueue channelStatusOnlyCmds = channelStatusOnly(device);
    for (CommandQueue::iterator i = channelStatusOnlyCmds.begin(); i != channelStatusOnlyCmds.end(); i++)
    {
        cmds.push_back(*i);
    }
    CommandQueue voltageDivCmds = voltageDiv(device);
    for (CommandQueue::iterator i = voltageDivCmds.begin(); i != voltageDivCmds.end(); i++)
    {
        cmds.push_back(*i);
    }
    CommandQueue timeBaseCmds = getTimebase(device);
    for (CommandQueue::iterator i = timeBaseCmds.begin(); i != timeBaseCmds.end(); i++)
    {
        cmds.push_back(*i);
    }
    // CommandQueue timeBaseCmds = trigger();
    // for (CommandQueue::iterator i = timeBaseCmds.begin(); i != timeBaseCmds.end(); i++)
    // {
    //     cmds.push_back(*i);
    // }
    cmds.push_back(channel1Level(device));
    cmds.push_back(channel2Level(device));
    cmds.push_back(channel1Coupling(device));
    cmds.push_back(channel2Coupling(device));
    CommandQueue pullSamplesCmds = pullSamples(device);
    for (CommandQueue::iterator i = pullSamplesCmds.begin(); i != pullSamplesCmds.end(); i++)
    {
        cmds.push_back(*i);
    }
    return cmds;
}

CommandQueue CommandGenerator::voltageDiv(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.push_back(updateChannelVolts125(device));
    cmds.push_back(relay1(device));
    cmds.push_back(relay2(device));
    cmds.push_back(channel1Level(device));
    return cmds;
}
CommandQueue CommandGenerator::pullSamples(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.push_back(triggerMode(device));
    uint8_t cmd[4] = {0xaa, 0x04, 0x00, 0x00};
    cmds.push_back(new Command(cmd));
    return cmds;
}
CommandQueue CommandGenerator::channelStatus(IDSO1070A &device)
{
    CommandQueue cmds;
    CommandQueue channelStatusOnlyCmds = channelStatusOnly(device);
    for (CommandQueue::iterator i = channelStatusOnlyCmds.begin(); i != channelStatusOnlyCmds.end(); i++)
    {
        cmds.push_back(*i);
    }
    CommandQueue timeBaseCmds = getTimebase(device);
    for (CommandQueue::iterator i = timeBaseCmds.begin(); i != timeBaseCmds.end(); i++)
    {
        cmds.push_back(*i);
    }
    return cmds;
}
CommandQueue CommandGenerator::channelStatusOnly(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.push_back(selectChannel(device));
    cmds.push_back(selectRAMChannel(device));
    cmds.push_back(readRamCount(device));
    return cmds;
}

Command *CommandGenerator::selectRAMChannel(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)CHANNEL_SELECTION,
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
        0x55, (uint8_t)CHANNEL_VOLTS_DIV_125,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::relay1(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::relay2(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::relay3(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::relay4(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::channel1Level(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::channel2Level(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::channel1Coupling(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::channel2Coupling(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::triggerMode(IDSO1070A &device)
{
    return NULL;
}
Command *CommandGenerator::readRamCount(IDSO1070A &device)
{
    return NULL;
}