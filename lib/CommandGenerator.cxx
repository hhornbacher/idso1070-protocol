#include "CommandGenerator.h"

float CommandGenerator::mapValue(int i, float f, float f2, float f3, float f4)
{
    return (((((float)i) - f) / (f2 - f)) * (f4 - f3)) + f3;
}

float CommandGenerator::mapValue(float f, float f2, float f3, float f4, float f5)
{
    return (((f - f2) / (f3 - f2)) * (f5 - f4)) + f4;
}

CommandGenerator::CommandGenerator(IDSO1070A *device) : device(device)
{
}

Command *CommandGenerator::readEEROMPage(uint8_t address)
{
    uint8_t cmdBuffer[4] = {0xee, 0xaa, address, 0x00};
    Command *cmd = new Command(cmdBuffer);
    // cmd->setResponseCount(1);
    cmd->setName("readEEROMPage");
    return cmd;
}

Command *CommandGenerator::readFPGAVersion()
{
    uint8_t cmdBuffer[4] = {0xaa, 0x02, 0x00, 0x00};
    Command *cmd = new Command(cmdBuffer);
    cmd->setName("readFPGAVersion");
    return cmd;
}

Command *CommandGenerator::readBatteryLevel()
{
    uint8_t cmdBuffer[4] = {0x57, 0x03, 0x00, 0x00};
    Command *cmd = new Command(cmdBuffer);
    cmd->setName("readBatteryLevel");
    return cmd;
}

Command *CommandGenerator::updateSampleRate()
{
    uint8_t b = 0;

    switch (device->timeBase)
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
    if (!device->channel1.enabled || !device->channel2.enabled)
    {
        switch (device->timeBase)
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
    cmd->setName("updateSampleRate");
    return cmd;
}

Command *CommandGenerator::getFreqDivLowBytes()
{
    // newTimeBase = idso1070.timeBase;
    uint16_t freqDiv = (uint16_t)(device->getTimebaseFromFreqDiv() & 0xffff);
    Command *cmd = new Command(CMDCODE_FREQ_DIV_LOW, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
    cmd->setName("getFreqDivLowBytes");
    return cmd;
}

Command *CommandGenerator::getFreqDivHighBytes()
{
    uint16_t freqDiv = (uint16_t)((device->getTimebaseFromFreqDiv() >> 16) & 0xffff);
    Command *cmd = new Command(CMDCODE_FREQ_DIV_HIGH, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
    cmd->setName("getFreqDivHighBytes");
    return cmd;
}

Command *CommandGenerator::selectRAMChannel()
{
    uint8_t b = 0x01;
    if (device->channel1.enabled && !device->channel2.enabled)
    {
        b = 0x08;
    }
    else if (device->channel2.enabled && !device->channel1.enabled)
    {
        b = 0x09;
    }
    else if (device->channel2.enabled && device->channel1.enabled)
    {
        b = 0x00;
    }
    Command *cmd = new Command(CMDCODE_RAM_CHANNEL_SELECTION, b);
    cmd->setName("selectRAMChannel");
    return cmd;
}

Command *CommandGenerator::updateChannelVolts125()
{
    uint8_t b = 0;
    switch (device->channel1.verticalDiv)
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

    switch (device->channel2.verticalDiv)
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
    cmd->setName("updateChannelVolts125");
    return cmd;
}

Command *CommandGenerator::relay1()
{
    uint8_t b = 0;

    switch (device->channel1.verticalDiv)
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
    cmd->setName("relay1");
    return cmd;
}

Command *CommandGenerator::relay2()
{
    uint8_t b = 0;

    switch (device->channel1.verticalDiv)
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
    cmd->setName("relay2");
    return cmd;
}

Command *CommandGenerator::relay3()
{
    uint8_t b = 0;

    switch (device->channel1.verticalDiv)
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
    cmd->setName("relay3");
    return cmd;
}

Command *CommandGenerator::relay4()
{
    uint8_t b = 0;

    switch (device->channel1.verticalDiv)
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
    cmd->setName("relay4");
    return cmd;
}

Command *CommandGenerator::channel1Coupling()
{
    uint8_t b1 = 0, b2 = 0;
    if (device->channel1.coupling == COUPLING_GND)
    {
        b1 = 0xff;
        b2 = 0x01;
    }
    else if (device->channel1.coupling == COUPLING_DC)
    {
        b1 = 0xef;
    }
    else
    {
        b1 = 0x10;
    }
    Command *cmd = new Command(CMDCODE_SET_RELAY, b1, b2);
    cmd->setName("channel1Coupling");
    return cmd;
}

Command *CommandGenerator::channel2Coupling()
{
    uint8_t b1 = 0, b2 = 0;
    if (device->channel2.coupling == COUPLING_GND)
    {
        b1 = 0xff;
        b2 = 0x02;
    }
    else if (device->channel2.coupling == COUPLING_DC)
    {
        b1 = 0xfe;
    }
    else
    {
        b1 = 0x01;
    }
    Command *cmd = new Command(CMDCODE_SET_RELAY, b1, b2);
    cmd->setName("channel2Coupling");
    return cmd;
}

Command *CommandGenerator::updateTriggerMode()
{
    uint8_t b = 0;
    if (device->captureMode == CAPMODE_ROLL)
        b = (1 << 0);
    else if (device->captureMode != CAPMODE_NORMAL)
        b |= (1 << 3);
    if (device->trigger.mode == TRIGMODE_AUTO)
        b |= (1 << 1);
    else if (device->trigger.mode == TRIGMODE_SINGLE)
        b |= (1 << 2);
    if (device->scopeMode == SCOMODE_DIGITAL)
        b |= (1 << 4);
    Command *cmd = new Command(CMDCODE_TRIGGER_MODE, b);
    cmd->setName("updateTriggerMode");
    return cmd;
}

Command *CommandGenerator::readRamCount()
{
    uint16_t b = 0;
    if (device->channel1.enabled && device->channel2.enabled)
    {
        b = (uint16_t)device->getSamplesNumberOfOneFrame();
    }
    else if (device->channel1.enabled || device->channel2.enabled)
    {
        if (!device->isSampleRate200Mor250M())
            b = (uint16_t)(device->getSamplesNumberOfOneFrame() / 2);
        else
        {
            double x = ((double)device->getSamplesNumberOfOneFrame() * device->trigger.xPosition / 2) + ((double)device->getSamplesNumberOfOneFrame() * (1 - device->trigger.xPosition));
            b = (uint16_t)x;
        }
    }
    Command *cmd = new Command(CMDCODE_READ_RAM_COUNT, (uint8_t)(b & 0xff), (uint8_t)((b >> 8) & 0xf + ((device->getPacketsNumber() - 1) << 4)));
    cmd->setName("readRamCount");
    return cmd;
}

Command *CommandGenerator::channel1Level()
{
    int verticalDiv = (int)device->channel1.verticalDiv;
    Command *cmd = channel1PWM(
        (uint16_t)mapValue(
            device->channel1.verticalPosition,
            8.0f, 248.0f,
            (float)device->channel1.pwmArray[verticalDiv][0], (float)device->channel1.pwmArray[verticalDiv][1]));
    cmd->setName("channel1Level");
    return cmd;
}

Command *CommandGenerator::channel2Level()
{
    int verticalDiv = (int)device->channel2.verticalDiv;
    Command *cmd = channel2PWM(
        (uint16_t)mapValue(
            device->channel2.verticalPosition,
            8.0f, 248.0f,
            (float)device->channel2.pwmArray[verticalDiv][0], (float)device->channel2.pwmArray[verticalDiv][1]));
    cmd->setName("channel2Level");
    return cmd;
}

Command *CommandGenerator::updateTriggerSourceAndSlope()
{
    uint8_t b =
        device->trigger.channel == TRIGCHAN_CH1 ? 0x01 : device->trigger.channel == TRIGCHAN_CH2 ? 0x00 : device->trigger.channel == TRIGCHAN_EXT ? 0x02 : 0x03;

    b &= ~0x2c;
    if (device->scopeMode == SCOMODE_ANALOG)
        b |= 0x10;
    else
        b &= ~0x10;
    if (device->trigger.slope == TRIGSLOPE_RISING)
        b |= 0x80;
    else
        b &= ~0x80;

    Command *cmd = new Command(CMDCODE_TRIGGER_SOURCE, b);
    cmd->setName("updateTriggerSourceAndSlope");
    return cmd;
}

Command *CommandGenerator::updateTriggerLevel()
{
    // uint16_t pwm = mapValue(device->trigger.level, 8.0f, 248.0f, (float)device->trigger.getBottomPWM(), (float)device->trigger.getTopPWM());
    uint16_t pwm = 2741;
    Command *cmd = updateTriggerPWM(pwm);
    cmd->setName("updateTriggerLevel");
    return cmd;
}

Command *CommandGenerator::updateTriggerPWM(uint16_t pwm)
{
    if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
        return NULL;
    return new Command(CMDCODE_TRIGGER_PWM,
                       (uint8_t)(pwm & 0xff),
                       (uint8_t)((pwm >> 8) & 0x0f));
}

Command *CommandGenerator::selectChannel()
{
    uint8_t b = 0;
    if (device->channel1.enabled && !device->channel2.enabled && device->isSampleRate200Mor250M())
    {
        b = 0x00;
    }
    else if (device->channel2.enabled && !device->channel1.enabled && device->isSampleRate200Mor250M())
    {
        b = 0x01;
    }
    else if (device->getEnabledChannelsCount() == 2 || (!device->isSampleRate200Mor250M() && device->getEnabledChannelsCount() == 1))
    {
        b = 0x02;
    }
    else
    {
        b = 0x03;
    }
    Command *cmd = new Command(CMDCODE_CHANNEL_SELECTION, b);
    cmd->setName("selectChannel");
    return cmd;
}

Command *CommandGenerator::preTrigger()
{
    uint16_t i = ((uint16_t)(((double)device->getSamplesNumberOfOneFrame()) * device->trigger.xPosition)) + 5;
    Command *cmd = new Command(CMDCODE_PRE_TRIGGER_LENGTH,
                               (uint8_t)(i & 0xff),
                               (uint8_t)((i >> 8) & 0xff));
    cmd->setName("preTrigger");
    return cmd;
}

Command *CommandGenerator::postTrigger()
{
    uint16_t i = ((uint16_t)(((double)device->getSamplesNumberOfOneFrame()) * (1 - device->trigger.xPosition)));
    Command *cmd = new Command(CMDCODE_POST_TRIGGER_LENGTH,
                               (uint8_t)(i & 0xff),
                               (uint8_t)((i >> 8) & 0xff));
    cmd->setName("postTrigger");
    return cmd;
}

Command *CommandGenerator::startSampling()
{
    uint8_t cmdPayload[4] = {0xaa, 0x04, 0x00, 0x00};
    Command *cmd = new Command(cmdPayload);
    cmd->setName("startSampling");
    return cmd;
}

Command *CommandGenerator::channel1PWM(uint16_t pwm)
{
    if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
        return NULL;
    return new Command(CMDCODE_CH1_PWM,
                       (uint8_t)(pwm & 0xff),
                       (uint8_t)((pwm >> 8) & 0x0f));
}

Command *CommandGenerator::channel2PWM(uint16_t pwm)
{
    if (pwm < 0 || pwm > IDSO1070A_MAX_PWM)
        return NULL;
    return new Command(CMDCODE_CH2_PWM,
                       (uint8_t)(pwm & 0xff),
                       (uint8_t)((pwm >> 8) & 0x0f));
}