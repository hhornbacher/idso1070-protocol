#include "CommandFactory.h"

CommandFactory::CommandFactory(IDSO1070A &device) : device(device)
{
}

void CommandFactory::setHandler(Command::Command::ResponseHandler handler)
{
    this->handler = handler;
}

std::deque<CommandGenerator> &CommandFactory::init(bool internal)
{
    if (!internal)
        buffer.clear();
    for (auto cmd : channelStatusOnly())
        buffer.push_back(cmd);
    for (auto cmd : voltageDiv())
        buffer.push_back(cmd);
    for (auto cmd : updateTimeBase())
        buffer.push_back(cmd);
    for (auto cmd : trigger())
        buffer.push_back(cmd);
    buffer.push_back(channel1Level());
    buffer.push_back(channel2Level());
    buffer.push_back(channel1Coupling());
    buffer.push_back(channel2Coupling());
    for (auto cmd : pullSamples())
        buffer.push_back(cmd);
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::channelStatusOnly(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(selectChannel());
    buffer.push_back(selectRAMChannel());
    buffer.push_back(readRamCount());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::channelStatus(bool internal)
{
    if (!internal)
        buffer.clear();
    for (auto cmd : channelStatusOnly())
        buffer.push_back(cmd);
    for (auto cmd : updateTimeBase())
        buffer.push_back(cmd);
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::channel1VoltageDiv(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(updateChannelVolts125());
    buffer.push_back(relay1());
    buffer.push_back(relay2());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::channel2VoltageDiv(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(updateChannelVolts125());
    buffer.push_back(relay3());
    buffer.push_back(relay4());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::levels(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(channel1Level());
    buffer.push_back(channel2Level());
    buffer.push_back(updateTriggerLevel());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::pullSamples(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(updateTriggerMode());
    buffer.push_back(startSampling());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::readEEROMandFPGA(bool internal)
{
    buffer.clear();
    buffer.push_back(readFPGAVersion());
    buffer.push_back(readEEROMPage(0x00));
    buffer.push_back(readEEROMPage(0x04));
    buffer.push_back(readEEROMPage(0x07));
    buffer.push_back(readEEROMPage(0x08));
    buffer.push_back(readEEROMPage(0x09));
    buffer.push_back(readEEROMPage(0x0a));
    buffer.push_back(readEEROMPage(0x0b));
    buffer.push_back(readEEROMPage(0x0c));
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::updateTimeBase(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(updateSampleRate());
    buffer.push_back(getFreqDivLowBytes());
    buffer.push_back(getFreqDivHighBytes());
    for (auto cmd : updateXTriggerPos())
        buffer.push_back(cmd);
    buffer.push_back(readRamCount());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::trigger(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(updateTriggerSourceAndSlope());
    buffer.push_back(updateTriggerMode());
    buffer.push_back(updateTriggerLevel());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::triggerSource(bool internal)
{
    buffer.clear();
    buffer.push_back(updateTriggerSourceAndSlope());
    buffer.push_back(updateTriggerLevel());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::voltageDiv(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(updateChannelVolts125());
    buffer.push_back(relay1());
    buffer.push_back(relay2());
    buffer.push_back(relay3());
    buffer.push_back(relay4());
    buffer.push_back(channel1Level());
    buffer.push_back(channel2Level());
    return buffer;
}

std::deque<CommandGenerator> &CommandFactory::updateXTriggerPos(bool internal)
{
    if (!internal)
        buffer.clear();
    buffer.push_back(preTrigger());
    buffer.push_back(postTrigger());
    return buffer;
}

CommandGenerator CommandFactory::readEEROMPage(uint8_t address)
{
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, address]() {
        uint8_t cmdBuffer[4] = {0xee, 0xaa, address, 0x00};
        Command *cmd = new Command(cmdBuffer);
        cmd->setHandler(handlerRef);
        // cmd->setResponseCount(1);
        cmd->setName("readEEROMPage");
        return cmd;
    };
}

CommandGenerator CommandFactory::readFPGAVersion()
{
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef]() {
        uint8_t cmdBuffer[4] = {0xaa, 0x02, 0x00, 0x00};
        Command *cmd = new Command(cmdBuffer);
        cmd->setHandler(handlerRef);
        cmd->setName("readFPGAVersion");
        return cmd;
    };
}

CommandGenerator CommandFactory::readBatteryLevel()
{
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef]() {
        uint8_t cmdBuffer[4] = {0x57, 0x03, 0x00, 0x00};
        Command *cmd = new Command(cmdBuffer);
        cmd->setHandler(handlerRef);
        cmd->setName("readBatteryLevel");
        return cmd;
    };
}

CommandGenerator CommandFactory::updateSampleRate()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;

        switch (devRef.getTimeBase())
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
        if (!devRef.getChannel1().isEnabled() || !devRef.getChannel2().isEnabled())
        {
            switch (devRef.getTimeBase())
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
        Command *cmd = new Command(CMDCODE_SAMPLE_RATE, b);
        cmd->setHandler(handlerRef);
        cmd->setName("updateSampleRate");
        return cmd;
    };
}

CommandGenerator CommandFactory::getFreqDivLowBytes()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        // newTimeBase = idso1070.timeBase;
        uint16_t freqDiv = (uint16_t)(devRef.getTimebaseFromFreqDiv() & 0xffff);
        Command *cmd = new Command(CMDCODE_FREQ_DIV_LOW, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
        cmd->setHandler(handlerRef);
        cmd->setName("getFreqDivLowBytes");
        return cmd;
    };
}

CommandGenerator CommandFactory::getFreqDivHighBytes()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint16_t freqDiv = (uint16_t)((devRef.getTimebaseFromFreqDiv() >> 16) & 0xffff);
        Command *cmd = new Command(CMDCODE_FREQ_DIV_HIGH, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
        cmd->setHandler(handlerRef);
        cmd->setName("getFreqDivHighBytes");
        return cmd;
    };
}

CommandGenerator CommandFactory::selectRAMChannel()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0x01;
        if (devRef.getChannel1().isEnabled() && !devRef.getChannel2().isEnabled())
        {
            b = 0x08;
        }
        else if (devRef.getChannel2().isEnabled() && !devRef.getChannel1().isEnabled())
        {
            b = 0x09;
        }
        else if (devRef.getChannel2().isEnabled() && devRef.getChannel1().isEnabled())
        {
            b = 0x00;
        }
        Command *cmd = new Command(CMDCODE_RAM_CHANNEL_SELECTION, b);
        cmd->setHandler(handlerRef);
        cmd->setName("selectRAMChannel");
        return cmd;
    };
}

CommandGenerator CommandFactory::updateChannelVolts125()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;
        switch (devRef.getChannel1().getVerticalDiv())
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

        switch (devRef.getChannel2().getVerticalDiv())
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
        Command *cmd = new Command(CMDCODE_CHANNEL_VOLTS_DIV_125, b);
        cmd->setHandler(handlerRef);
        cmd->setName("updateChannelVolts125");
        return cmd;
    };
}

CommandGenerator CommandFactory::relay1()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;

        switch (devRef.getChannel1().getVerticalDiv())
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
        Command *cmd = new Command(CMDCODE_SET_RELAY, b);
        cmd->setHandler(handlerRef);
        cmd->setName("relay1");
        return cmd;
    };
}

CommandGenerator CommandFactory::relay2()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;

        switch (devRef.getChannel1().getVerticalDiv())
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

        Command *cmd = new Command(CMDCODE_SET_RELAY, b);
        cmd->setHandler(handlerRef);
        cmd->setName("relay2");
        return cmd;
    };
}

CommandGenerator CommandFactory::relay3()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;

        switch (devRef.getChannel1().getVerticalDiv())
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

        Command *cmd = new Command(CMDCODE_SET_RELAY, b);
        cmd->setHandler(handlerRef);
        cmd->setName("relay3");
        return cmd;
    };
}

CommandGenerator CommandFactory::relay4()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;

        switch (devRef.getChannel1().getVerticalDiv())
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

        Command *cmd = new Command(CMDCODE_SET_RELAY, b);
        cmd->setHandler(handlerRef);
        cmd->setName("relay4");
        return cmd;
    };
}

CommandGenerator CommandFactory::channel1Coupling()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b1 = 0, b2 = 0;
        if (devRef.getChannel1().getCoupling() == COUPLING_GND)
        {
            b1 = 0xff;
            b2 = 0x01;
        }
        else if (devRef.getChannel1().getCoupling() == COUPLING_DC)
        {
            b1 = 0xef;
        }
        else
        {
            b1 = 0x10;
        }
        Command *cmd = new Command(CMDCODE_SET_RELAY, b1, b2);
        cmd->setHandler(handlerRef);
        cmd->setName("channel1Coupling");
        return cmd;
    };
}

CommandGenerator CommandFactory::channel2Coupling()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b1 = 0, b2 = 0;
        if (devRef.getChannel2().getCoupling() == COUPLING_GND)
        {
            b1 = 0xff;
            b2 = 0x02;
        }
        else if (devRef.getChannel2().getCoupling() == COUPLING_DC)
        {
            b1 = 0xfe;
        }
        else
        {
            b1 = 0x01;
        }
        Command *cmd = new Command(CMDCODE_SET_RELAY, b1, b2);
        cmd->setHandler(handlerRef);
        cmd->setName("channel2Coupling");
        return cmd;
    };
}

CommandGenerator CommandFactory::updateTriggerMode()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;
        if (devRef.getCaptureMode() == CAPMODE_ROLL)
            b = (1 << 0);
        else if (devRef.getCaptureMode() != CAPMODE_NORMAL)
            b |= (1 << 3);
        if (devRef.getTrigger().getMode() == TRIGMODE_AUTO)
            b |= (1 << 1);
        else if (devRef.getTrigger().getMode() == TRIGMODE_SINGLE)
            b |= (1 << 2);
        if (devRef.getScopeMode() == SCOMODE_DIGITAL)
            b |= (1 << 4);
        Command *cmd = new Command(CMDCODE_TRIGGER_MODE, b);
        cmd->setHandler(handlerRef);
        cmd->setName("updateTriggerMode");
        return cmd;
    };
}

CommandGenerator CommandFactory::readRamCount()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint16_t b = 0;
        if (devRef.getChannel1().isEnabled() && devRef.getChannel2().isEnabled())
        {
            b = (uint16_t)devRef.getSamplesNumberOfOneFrame();
        }
        else if (devRef.getChannel1().isEnabled() || devRef.getChannel2().isEnabled())
        {
            if (!devRef.isSampleRate200Mor250M())
                b = (uint16_t)(devRef.getSamplesNumberOfOneFrame() / 2);
            else
            {
                double x = ((double)devRef.getSamplesNumberOfOneFrame() * devRef.getTrigger().getXPosition() / 2) + ((double)devRef.getSamplesNumberOfOneFrame() * (1 - devRef.getTrigger().getXPosition()));
                b = (uint16_t)x;
            }
        }
        Command *cmd = new Command(CMDCODE_READ_RAM_COUNT, (uint8_t)(b & 0xff), (uint8_t)((b >> 8) & 0xf + ((devRef.getPacketsNumber() - 1) << 4)));
        cmd->setHandler(handlerRef);
        cmd->setName("readRamCount");
        return cmd;
    };
}

CommandGenerator CommandFactory::channel1Level()
{
    int verticalDiv = (int)device.getChannel1().getVerticalDiv();
    return channel1PWM((uint16_t)mapValue(
        device.getChannel1().getVerticalPosition(),
        8.0f, 248.0f,
        (float)device.getChannel1().getPWM(verticalDiv, 0), (float)device.getChannel1().getPWM(verticalDiv, 1)));
}

CommandGenerator CommandFactory::channel2Level()
{
    int verticalDiv = (int)device.getChannel2().getVerticalDiv();
    return channel2PWM((uint16_t)mapValue(
        device.getChannel2().getVerticalPosition(),
        8.0f, 248.0f,
        (float)device.getChannel2().getPWM(verticalDiv, 0), (float)device.getChannel2().getPWM(verticalDiv, 1)));
}

CommandGenerator CommandFactory::updateTriggerSourceAndSlope()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b =
            devRef.getTrigger().getChannel() == TRIGCHAN_CH1 ? 0x01 : devRef.getTrigger().getChannel() == TRIGCHAN_CH2 ? 0x00 : devRef.getTrigger().getChannel() == TRIGCHAN_EXT ? 0x02 : 0x03;

        b &= ~0x2c;
        if (devRef.getScopeMode() == SCOMODE_ANALOG)
            b |= 0x10;
        else
            b &= ~0x10;
        if (devRef.getTrigger().getSlope() == TRIGSLOPE_RISING)
            b |= 0x80;
        else
            b &= ~0x80;

        Command *cmd = new Command(CMDCODE_TRIGGER_SOURCE, b);
        cmd->setHandler(handlerRef);
        cmd->setName("updateTriggerSourceAndSlope");
        return cmd;
    };
}

CommandGenerator CommandFactory::updateTriggerLevel()
{
    // uint16_t pwm = mapValue(device.getTrigger().level, 8.0f, 248.0f, (float)device.getTrigger().getBottomPWM(), (float)device.getTrigger().getTopPWM());
    uint16_t pwm = 2741;
    return updateTriggerPWM(pwm);
}

CommandGenerator CommandFactory::selectChannel()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint8_t b = 0;
        if (devRef.getChannel1().isEnabled() && !devRef.getChannel2().isEnabled() && devRef.isSampleRate200Mor250M())
        {
            b = 0x00;
        }
        else if (devRef.getChannel2().isEnabled() && !devRef.getChannel1().isEnabled() && devRef.isSampleRate200Mor250M())
        {
            b = 0x01;
        }
        else if (devRef.getEnabledChannelsCount() == 2 || (!devRef.isSampleRate200Mor250M() && devRef.getEnabledChannelsCount() == 1))
        {
            b = 0x02;
        }
        else
        {
            b = 0x03;
        }
        Command *cmd = new Command(CMDCODE_CHANNEL_SELECTION, b);
        cmd->setHandler(handlerRef);
        cmd->setName("selectChannel");
        return cmd;
    };
}

CommandGenerator CommandFactory::preTrigger()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint16_t i = ((uint16_t)(((double)devRef.getSamplesNumberOfOneFrame()) * devRef.getTrigger().getXPosition())) + 5;
        Command *cmd = new Command(CMDCODE_PRE_TRIGGER_LENGTH,
                                   (uint8_t)(i & 0xff),
                                   (uint8_t)((i >> 8) & 0xff));
        cmd->setHandler(handlerRef);
        cmd->setName("preTrigger");
        return cmd;
    };
}

CommandGenerator CommandFactory::postTrigger()
{
    IDSO1070A &devRef = device;
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, &devRef]() {
        uint16_t i = ((uint16_t)(((double)devRef.getSamplesNumberOfOneFrame()) * (1 - devRef.getTrigger().getXPosition())));
        Command *cmd = new Command(CMDCODE_POST_TRIGGER_LENGTH,
                                   (uint8_t)(i & 0xff),
                                   (uint8_t)((i >> 8) & 0xff));
        cmd->setHandler(handlerRef);
        cmd->setName("postTrigger");
        return cmd;
    };
}

CommandGenerator CommandFactory::startSampling()
{
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef]() {
        uint8_t cmdPayload[4] = {0xaa, 0x04, 0x00, 0x00};
        Command *cmd = new Command(cmdPayload);
        cmd->setHandler(handlerRef);
        cmd->setName("startSampling");
        return cmd;
    };
}

CommandGenerator CommandFactory::updateTriggerPWM(uint16_t pwm)
{
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, pwm]() {
        if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
            return (Command *)NULL;
        Command *cmd = new Command(CMDCODE_TRIGGER_PWM,
                                   (uint8_t)(pwm & 0xff),
                                   (uint8_t)((pwm >> 8) & 0x0f));
        cmd->setHandler(handlerRef);
        cmd->setName("updateTriggerPWM");
        return cmd;
    };
}

CommandGenerator CommandFactory::channel1PWM(uint16_t pwm)
{
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, pwm]() {
        if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
            return (Command *)NULL;
        Command *cmd = new Command(CMDCODE_CH1_PWM,
                                   (uint8_t)(pwm & 0xff),
                                   (uint8_t)((pwm >> 8) & 0x0f));
        cmd->setHandler(handlerRef);
        cmd->setName("channel1PWM");
        return cmd;
    };
}

CommandGenerator CommandFactory::channel2PWM(uint16_t pwm)
{
    Command::ResponseHandler handlerRef = handler;
    return [handlerRef, pwm]() {
        if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
            return (Command *)NULL;
        Command *cmd = new Command(CMDCODE_CH2_PWM,
                                   (uint8_t)(pwm & 0xff),
                                   (uint8_t)((pwm >> 8) & 0x0f));
        cmd->setHandler(handlerRef);
        cmd->setName("channel2PWM");
        return cmd;
    };
}