#include "CommandGenerator.h"

CommandQueue CommandGenerator::readEEROM(IDSO1070A &device)
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
    // cmds.add(trigger(device));
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
    cmds.add(channel1Level(device));
    return cmds;
}

CommandQueue CommandGenerator::pullSamples(IDSO1070A &device)
{
    CommandQueue cmds;
    cmds.add(triggerMode(device));
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
    cmds.add(triggerMode(device));
    cmds.add(updateTriggerLevel(device));
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
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)SAMPLE_RATE, b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::getFreqDivLowBytes(IDSO1070A &device)
{
    // newTimeBase = idso1070.timeBase;
    // CommandPacket commandPacket = new CommandPacket();
    // commandPacket.setCommand(Commands.FREQ_DIV_LOW);
    // long freqDiv = getFreqDiv(idso1070.timeBase);
    // commandPacket.setData(2, (int) ((freqDiv & 65535) & 255));
    // commandPacket.setData(3, (int) ((freqDiv & 65535) >> 8));
    // return commandPacket;
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)FREQ_DIV_LOW, 0x00, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::getFreqDivHighBytes(IDSO1070A &device)
{
    // CommandPacket commandPacket = new CommandPacket();
    // commandPacket.setCommand(Commands.FREQ_DIV_HIGH);
    // int freqDiv = (int) ((getFreqDiv(idso1070.timeBase) & -65536) >> 16);
    // commandPacket.setData(2, freqDiv & 255);
    // commandPacket.setData(3, (freqDiv >> 8) & 255);
    // return commandPacket;
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)FREQ_DIV_HIGH, 0x04, 0x00};
    return new Command(cmdBuffer);
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
        0x55, (uint8_t)SET_RELAY,
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
        0x55, (uint8_t)SET_RELAY,
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
        0x55, (uint8_t)SET_RELAY,
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
        0x55, (uint8_t)SET_RELAY,
        b, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandGenerator::channel1Coupling(IDSO1070A &device)
{
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)SET_RELAY,
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
        0x55, (uint8_t)SET_RELAY,
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

Command *CommandGenerator::triggerMode(IDSO1070A &device)
{
    uint8_t b = 0;
    if (device.captureMode == CAPMODE_ROLL)
        b = 0x01;
    // byteBit = idso1070.getTrigger().getTriggerMode() == TriggerMode.AUTO ? Utility.setByteBit(byteBit, 1) : Utility.unsetByteBit(byteBit, 1);
    // byteBit = idso1070.getTrigger().getTriggerMode() == TriggerMode.SINGLE ? Utility.setByteBit(byteBit, 2) : Utility.unsetByteBit(byteBit, 2);
    else if (device.captureMode != CAPMODE_NORMAL)
        b = 0x08;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)TRIGGER_MODE,
        b, 0x00};
    return new Command(cmdBuffer);

    // CommandPacket commandPacket = new CommandPacket(Commands.TRIGGER_MODE);
    // byte byteBit = idso1070.getCaptureMode() == CaptureMode.ROLL ? Utility.setByteBit((byte) 0, 0) : Utility.unsetByteBit((byte) 0, 0);
    // byteBit = idso1070.getTrigger().getTriggerMode() == TriggerMode.AUTO ? Utility.setByteBit(byteBit, 1) : Utility.unsetByteBit(byteBit, 1);
    // byteBit = idso1070.getTrigger().getTriggerMode() == TriggerMode.SINGLE ? Utility.setByteBit(byteBit, 2) : Utility.unsetByteBit(byteBit, 2);
    // byteBit = idso1070.getCaptureMode() == CaptureMode.NORMAL ? Utility.unsetByteBit(byteBit, 3) : Utility.setByteBit(byteBit, 3);
    // commandPacket.setData(2, Utility.unsetByteBit(idso1070.getScopeMode() == ScopeMode.DIGITAL ? Utility.setByteBit(byteBit, 4) : Utility.unsetByteBit(byteBit, 4), 5));
    // commandPacket.setData(3, 0);
    // return commandPacket;
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
            double x = ((double)device.getSamplesNumberOfOneFrame() * 1 /*idso1070.trigger.getXPosition()*/ / 2) + ((double)device.getSamplesNumberOfOneFrame() * (1 - 1 /*idso1070.trigger.getXPosition()*/));
            b = (uint16_t)x;
        }
    }
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)READ_RAM_COUNT,
        (uint8_t)(b & 0xff),
        (uint8_t)((b >> 8) & 0xf + ((device.getPacketsNumber() - 1) << 4))};
    return new Command(cmdBuffer);
    return NULL;
}

Command *CommandGenerator::channel1Level(IDSO1070A &device)
{
    // int ordinal = idso1070.channel1.getVerticalDiv().ordinal();
    // return getCh1PwmCommand((int) Utility.Map(idso1070.channel1.getVerticalPosition(), 8.0f, 248.0f, (float) idso1070.channel1.getPwmArray()[ordinal][0], (float) idso1070.channel1.getPwmArray()[ordinal][1]));
    return NULL;
}
Command *CommandGenerator::channel2Level(IDSO1070A &device)
{
    return NULL;
}

Command *CommandGenerator::updateTriggerSourceAndSlope(IDSO1070A &device)
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

Command *CommandGenerator::updateTriggerLevel(IDSO1070A &device)
{
    return updateTriggerPWM(0x91);
    // return updateTriggerPWM((int) Utility.Map(idso1070.trigger.updateTriggerLevel(), 8.0f, 248.0f, (float) idso1070.trigger.getBottomPwm(), (float) idso1070.trigger.getTopPwm()));
}

Command *CommandGenerator::updateTriggerPWM(int i)
{
    if (i < 0) //!!!
        return NULL;
    uint8_t cmdBuffer[4] = {
        0x55, (uint8_t)TRIGGER_PWM,
        (uint8_t)(i & 0xff),
        (uint8_t)((i >> 8) & 0x0f)};
    return new Command(cmdBuffer);
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

Command *CommandGenerator::preTrigger(IDSO1070A &device)
{
    // CommandPacket commandPacket = new CommandPacket(Commands.PRE_TRIGGER_LENGTH);
    // int samplesNumberOfOneFrame = ((int) (((double) idso1070.getSamplesNumberOfOneFrame()) * idso1070.getTrigger().getXPosition())) + 5;
    // commandPacket.setData(2, samplesNumberOfOneFrame & 255);
    // commandPacket.setData(3, (samplesNumberOfOneFrame >> 8) & 255);
    // return commandPacket;
}
Command *CommandGenerator::postTrigger(IDSO1070A &device)
{
    // CommandPacket commandPacket = new CommandPacket(Commands.POST_TRIGGER_LENGTH);
    // int samplesNumberOfOneFrame = (int) (((double) idso1070.getSamplesNumberOfOneFrame()) * (1.0d - idso1070.getTrigger().getXPosition()));
    // commandPacket.setData(2, samplesNumberOfOneFrame & 255);
    // commandPacket.setData(3, (samplesNumberOfOneFrame >> 8) & 255);
    // return commandPacket;
}