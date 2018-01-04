#include "CommandGenerator.h"

CommandGenerator::CommandGenerator(IDSO1070A &device) : device(device)
{
}

std::deque<CommandGenerationFunction> &CommandGenerator::init(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    for (auto cmd : channelStatusOnly(nullHandler))
        buffer.push_back(cmd);
    for (auto cmd : voltageDiv(nullHandler))
        buffer.push_back(cmd);
    for (auto cmd : updateTimeBase(nullHandler))
        buffer.push_back(cmd);
    for (auto cmd : trigger(nullHandler))
        buffer.push_back(cmd);
    buffer.push_back(channel1Level(nullHandler));
    buffer.push_back(channel2Level(nullHandler));
    buffer.push_back(channel1Coupling(nullHandler));
    buffer.push_back(channel2Coupling(nullHandler));
    for (auto cmd : pullSamples(handler))
        buffer.push_back(cmd);
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::channelStatusOnly(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::channelStatus(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::channel1VoltageDiv(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::channel2VoltageDiv(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::levels(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::pullSamples(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::readEEROMandFPGA(CommandResponseHandler handler, bool internal)
{
    buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::updateTimeBase(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::trigger(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::triggerSource(CommandResponseHandler handler, bool internal)
{
    buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::voltageDiv(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

std::deque<CommandGenerationFunction> &CommandGenerator::updateXTriggerPos(CommandResponseHandler handler, bool internal)
{
    if (!internal)
        buffer.clear();
    return buffer;
}

CommandGenerationFunction CommandGenerator::readEEROMPage(CommandResponseHandler handler, uint8_t address)
{
    return [handler, address]() {
        uint8_t cmdBuffer[4] = {0xee, 0xaa, address, 0x00};
        Command *cmd = new Command(cmdBuffer);
        cmd->setCommandHandler(handler);
        // cmd->setResponseCount(1);
        cmd->setName("readEEROMPage");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::readFPGAVersion(CommandResponseHandler handler)
{
    return [handler]() {
        uint8_t cmdBuffer[4] = {0xaa, 0x02, 0x00, 0x00};
        Command *cmd = new Command(cmdBuffer);
        cmd->setCommandHandler(handler);
        cmd->setName("readFPGAVersion");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::readBatteryLevel(CommandResponseHandler handler)
{
    return [handler]() {
        uint8_t cmdBuffer[4] = {0x57, 0x03, 0x00, 0x00};
        Command *cmd = new Command(cmdBuffer);
        cmd->setCommandHandler(handler);
        cmd->setName("readBatteryLevel");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::updateSampleRate(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("updateSampleRate");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::getFreqDivLowBytes(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
        // newTimeBase = idso1070.timeBase;
        uint16_t freqDiv = (uint16_t)(devRef.getTimebaseFromFreqDiv() & 0xffff);
        Command *cmd = new Command(CMDCODE_FREQ_DIV_LOW, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
        cmd->setCommandHandler(handler);
        cmd->setName("getFreqDivLowBytes");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::getFreqDivHighBytes(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
        uint16_t freqDiv = (uint16_t)((devRef.getTimebaseFromFreqDiv() >> 16) & 0xffff);
        Command *cmd = new Command(CMDCODE_FREQ_DIV_HIGH, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
        cmd->setCommandHandler(handler);
        cmd->setName("getFreqDivHighBytes");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::selectRAMChannel(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("selectRAMChannel");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::updateChannelVolts125(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("updateChannelVolts125");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::relay1(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("relay1");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::relay2(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("relay2");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::relay3(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("relay3");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::relay4(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("relay4");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::channel1Coupling(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("channel1Coupling");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::channel2Coupling(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("channel2Coupling");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::updateTriggerMode(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("updateTriggerMode");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::readRamCount(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("readRamCount");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::channel1Level(CommandResponseHandler handler)
{
    int verticalDiv = (int)device.getChannel1().getVerticalDiv();
    return channel1PWM(handler,
                       (uint16_t)mapValue(
                           device.getChannel1().getVerticalPosition(),
                           8.0f, 248.0f,
                           (float)device.getChannel1().getPWM(verticalDiv, 0), (float)device.getChannel1().getPWM(verticalDiv, 1)));
}

CommandGenerationFunction CommandGenerator::channel2Level(CommandResponseHandler handler)
{
    int verticalDiv = (int)device.getChannel2().getVerticalDiv();
    return channel2PWM(handler,
                       (uint16_t)mapValue(
                           device.getChannel2().getVerticalPosition(),
                           8.0f, 248.0f,
                           (float)device.getChannel2().getPWM(verticalDiv, 0), (float)device.getChannel2().getPWM(verticalDiv, 1)));
}

CommandGenerationFunction CommandGenerator::updateTriggerSourceAndSlope(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("updateTriggerSourceAndSlope");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::updateTriggerLevel(CommandResponseHandler handler)
{
    // uint16_t pwm = mapValue(device.getTrigger().level, 8.0f, 248.0f, (float)device.getTrigger().getBottomPWM(), (float)device.getTrigger().getTopPWM());
    uint16_t pwm = 2741;
    return updateTriggerPWM(handler, pwm);
}

CommandGenerationFunction CommandGenerator::selectChannel(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
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
        cmd->setCommandHandler(handler);
        cmd->setName("selectChannel");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::preTrigger(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
        uint16_t i = ((uint16_t)(((double)devRef.getSamplesNumberOfOneFrame()) * devRef.getTrigger().getXPosition())) + 5;
        Command *cmd = new Command(CMDCODE_PRE_TRIGGER_LENGTH,
                                   (uint8_t)(i & 0xff),
                                   (uint8_t)((i >> 8) & 0xff));
        cmd->setCommandHandler(handler);
        cmd->setName("preTrigger");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::postTrigger(CommandResponseHandler handler)
{
    IDSO1070A &devRef = device;
    return [handler, &devRef]() {
        uint16_t i = ((uint16_t)(((double)devRef.getSamplesNumberOfOneFrame()) * (1 - devRef.getTrigger().getXPosition())));
        Command *cmd = new Command(CMDCODE_POST_TRIGGER_LENGTH,
                                   (uint8_t)(i & 0xff),
                                   (uint8_t)((i >> 8) & 0xff));
        cmd->setCommandHandler(handler);
        cmd->setName("postTrigger");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::startSampling(CommandResponseHandler handler)
{
    return [handler]() {
        uint8_t cmdPayload[4] = {0xaa, 0x04, 0x00, 0x00};
        Command *cmd = new Command(cmdPayload);
        cmd->setCommandHandler(handler);
        cmd->setName("startSampling");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::updateTriggerPWM(CommandResponseHandler handler, uint16_t pwm)
{
    return [handler, pwm]() {
        if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
            return (Command *)NULL;
        Command *cmd = new Command(CMDCODE_TRIGGER_PWM,
                                   (uint8_t)(pwm & 0xff),
                                   (uint8_t)((pwm >> 8) & 0x0f));
        cmd->setCommandHandler(handler);
        cmd->setName("updateTriggerPWM");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::channel1PWM(CommandResponseHandler handler, uint16_t pwm)
{
    return [handler, pwm]() {
        if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
            return (Command *)NULL;
        Command *cmd = new Command(CMDCODE_CH1_PWM,
                                   (uint8_t)(pwm & 0xff),
                                   (uint8_t)((pwm >> 8) & 0x0f));
        cmd->setCommandHandler(handler);
        cmd->setName("channel1PWM");
        return cmd;
    };
}

CommandGenerationFunction CommandGenerator::channel2PWM(CommandResponseHandler handler, uint16_t pwm)
{
    return [handler, pwm]() {
        if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
            return (Command *)NULL;
        Command *cmd = new Command(CMDCODE_CH2_PWM,
                                   (uint8_t)(pwm & 0xff),
                                   (uint8_t)((pwm >> 8) & 0x0f));
        cmd->setCommandHandler(handler);
        cmd->setName("channel2PWM");
        return cmd;
    };
}