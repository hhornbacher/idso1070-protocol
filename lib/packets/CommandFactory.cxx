#include "packets/CommandFactory.h"

Command *CommandFactory::readEEROMPage(uint8_t address)
{
    uint8_t cmdBuffer[4] = {TYPE_EEROM, 0xaa, address, 0x00};
    Command *cmd = new Command(cmdBuffer);
    return cmd;
}

Command *CommandFactory::readARMVersion()
{
    uint8_t cmdBuffer[4] = {TYPE_STATE, 0x04, 0x00, 0x00};
    Command *cmd = new Command(cmdBuffer);
    return cmd;
}

Command *CommandFactory::readFPGAVersion()
{
    uint8_t cmdBuffer[4] = {TYPE_CONTROL, 0x02, 0x00, 0x00};
    Command *cmd = new Command(cmdBuffer);
    return cmd;
}

Command *CommandFactory::readBatteryLevel()
{
    uint8_t cmdBuffer[4] = {TYPE_STATE, 0x03, 0x00, 0x00};
    Command *cmd = new Command(cmdBuffer);
    return cmd;
}

Command *CommandFactory::readRamCount(int enabledChannels, uint16_t samplesNumberOfOneFrame,
                                      bool isSampleRate200Mor250M, double triggerXPosition,
                                      uint8_t packetsNumber)
{
    uint16_t b = 0;
    if (enabledChannels == 2)
    {
        b = samplesNumberOfOneFrame;
    }
    else if (enabledChannels == 1)
    {
        if (!isSampleRate200Mor250M)
            b = samplesNumberOfOneFrame / 2;
        else
        {
            double x = ((double)samplesNumberOfOneFrame * triggerXPosition / 2) +
                       ((double)samplesNumberOfOneFrame * (1 - triggerXPosition));
            b = (uint16_t)x;
        }
    }
    Command *cmd = new Command(CMDCODE_READ_RAM_COUNT,
                               (uint8_t)(b & 0xff),
                               (uint8_t)((b >> 8) & 0xf + ((packetsNumber - 1) << 4)));
    return cmd;
}

Command *CommandFactory::startSampling()
{
    uint8_t cmdPayload[4] = {TYPE_CONTROL, 0x04, 0x00, 0x00};
    Command *cmd = new Command(cmdPayload);
    return cmd;
}

Command *CommandFactory::updateSampleRate(TimeBase timeBase, int enabledChannels)
{
    uint8_t b = 0;

    switch (timeBase)
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
    if (enabledChannels == 1)
    {
        switch (timeBase)
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
    return cmd;
}

Command *CommandFactory::updateFreqDivLowBytes(TimeBase timeBase)
{
    uint16_t freqDiv = (uint16_t)(timeBase & 0xffff);
    Command *cmd = new Command(CMDCODE_FREQ_DIV_LOW, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
    return cmd;
}

Command *CommandFactory::updateFreqDivHighBytes(TimeBase timeBase)
{
    uint16_t freqDiv = (uint16_t)((timeBase >> 16) & 0xffff);
    Command *cmd = new Command(CMDCODE_FREQ_DIV_HIGH, (uint8_t)(freqDiv & 0xff), (uint8_t)((freqDiv >> 8) & 0xff));
    return cmd;
}

Command *CommandFactory::updateRAMChannelSelection(bool channel1Enabled, bool channel2Enabled)
{
    uint8_t b = 0x01;
    if (channel1Enabled && !channel2Enabled)
    {
        b = 0x08;
    }
    else if (channel2Enabled && !channel1Enabled)
    {
        b = 0x09;
    }
    else if (channel2Enabled && channel1Enabled)
    {
        b = 0x00;
    }
    Command *cmd = new Command(CMDCODE_RAM_CHANNEL_SELECTION, b);
    return cmd;
}

Command *CommandFactory::updateChannelVolts125(VoltageDiv channel1VerticalDiv, VoltageDiv channel2VerticalDiv)
{
    uint8_t b = 0;
    switch (channel1VerticalDiv)
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

    switch (channel2VerticalDiv)
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
    return cmd;
}

Command *CommandFactory::updateRelay1(VoltageDiv channel1VerticalDiv)
{
    uint8_t b = 0;

    switch (channel1VerticalDiv)
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
    return cmd;
}

Command *CommandFactory::updateRelay2(VoltageDiv channel1VerticalDiv)
{
    uint8_t b = 0;

    switch (channel1VerticalDiv)
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
    return cmd;
}

Command *CommandFactory::updateRelay3(VoltageDiv channel2VerticalDiv)
{
    uint8_t b = 0;

    switch (channel2VerticalDiv)
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
    return cmd;
}

Command *CommandFactory::updateRelay4(VoltageDiv channel2VerticalDiv)
{
    uint8_t b = 0;

    switch (channel2VerticalDiv)
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
    return cmd;
}

Command *CommandFactory::updateChannel1Coupling(InputCoupling channel1Coupling)
{
    uint8_t b1 = 0, b2 = 0;
    if (channel1Coupling == COUPLING_GND)
    {
        b1 = 0xff;
        b2 = 0x01;
    }
    else if (channel1Coupling == COUPLING_DC)
    {
        b1 = 0xef;
    }
    else
    {
        b1 = 0x10;
    }
    Command *cmd = new Command(CMDCODE_SET_RELAY, b1, b2);
    return cmd;
}

Command *CommandFactory::updateChannel2Coupling(InputCoupling channel2Coupling)
{
    uint8_t b1 = 0, b2 = 0;
    if (channel2Coupling == COUPLING_GND)
    {
        b1 = 0xff;
        b2 = 0x02;
    }
    else if (channel2Coupling == COUPLING_DC)
    {
        b1 = 0xfe;
    }
    else
    {
        b1 = 0x01;
    }
    Command *cmd = new Command(CMDCODE_SET_RELAY, b1, b2);
    return cmd;
}

Command *CommandFactory::updateTriggerMode(CaptureMode capMode, TriggerMode trigMode, ScopeMode scoMode)
{
    uint8_t b = 0;
    if (capMode == CAPMODE_ROLL)
        b = (1 << 0);
    else if (capMode != CAPMODE_NORMAL)
        b |= (1 << 3);
    if (trigMode == TRIGMODE_AUTO)
        b |= (1 << 1);
    else if (trigMode == TRIGMODE_SINGLE)
        b |= (1 << 2);
    if (scoMode == SCOMODE_DIGITAL)
        b |= (1 << 4);
    Command *cmd = new Command(CMDCODE_TRIGGER_MODE, b);
    return cmd;
}

Command *CommandFactory::updateChannel1Level(VoltageDiv channel1VerticalDiv, int16_t channel1VerticalPosition, float channel1PWM0, float channel1PWM1)
{
    int verticalDiv = (int)channel1VerticalDiv;
    uint16_t pwm = (uint16_t)mapValue(
        channel1VerticalPosition,
        8.0f, 248.0f,
        channel1PWM0, channel1PWM1);
    if (pwm > IDSO1070::MaxPWM)
        return (Command *)NULL;
    return new Command(CMDCODE_CH1_PWM,
                       (uint8_t)(pwm & 0xff),
                       (uint8_t)((pwm >> 8) & 0x0f));
}

Command *CommandFactory::updateChannel2Level(VoltageDiv channel2VerticalDiv, int16_t channel2VerticalPosition, float channel2PWM0, float channel2PWM1)
{
    int verticalDiv = (int)channel2VerticalDiv;
    uint16_t pwm = (uint16_t)mapValue(
        channel2VerticalPosition,
        8.0f, 248.0f,
        channel2PWM0, channel2PWM1);

    if (pwm > IDSO1070::MaxPWM)
        return (Command *)NULL;
    return new Command(CMDCODE_CH2_PWM,
                       (uint8_t)(pwm & 0xff),
                       (uint8_t)((pwm >> 8) & 0x0f));
}

Command *CommandFactory::updateTriggerSourceAndSlope(TriggerChannel triggerChanel, ScopeMode scoMode, TriggerSlope triggerSlope)
{
    uint8_t b =
        triggerChanel == TRIGCHAN_CH1 ? 0x01 : triggerChanel == TRIGCHAN_CH2 ? 0x00 : triggerChanel == TRIGCHAN_EXT ? 0x02 : 0x03;

    b &= ~0x2c;
    if (scoMode == SCOMODE_ANALOG)
        b |= 0x10;
    else
        b &= ~0x10;
    if (triggerSlope == TRIGSLOPE_RISING)
        b |= 0x80;
    else
        b &= ~0x80;

    return new Command(CMDCODE_TRIGGER_SOURCE, b);
}

Command *CommandFactory::updateTriggerLevel(uint16_t triggerLevel, float triggerTopPWM, float triggerBottomPWM)
{
    uint16_t pwm = mapValue(triggerLevel, 8.0f, 248.0f, triggerBottomPWM, triggerTopPWM);

    if (pwm > IDSO1070::MaxPWM)
        return (Command *)NULL;
    return new Command(CMDCODE_TRIGGER_PWM,
                       (uint8_t)(pwm & 0xff),
                       (uint8_t)((pwm >> 8) & 0x0f));
}

Command *CommandFactory::updateChannelSelection(bool channel1Enabled, bool channel2Enabled, bool isSampleRate200Mor250M)
{
    uint8_t b = 0;
    int channelCount = channel1Enabled && channel2Enabled ? 2 : channel1Enabled || channel2Enabled ? 1 : 0;
    if (channel1Enabled && !channel2Enabled && isSampleRate200Mor250M)
    {
        b = 0x00;
    }
    else if (channel2Enabled && !channel1Enabled && isSampleRate200Mor250M)
    {
        b = 0x01;
    }
    else if (channelCount == 2 || (!isSampleRate200Mor250M && (channelCount == 1)))
    {
        b = 0x02;
    }
    else
    {
        b = 0x03;
    }
    Command *cmd = new Command(CMDCODE_CHANNEL_SELECTION, b);
    return cmd;
}

Command *CommandFactory::updatePreTriggerLength(uint16_t samplesNumberOfOneFrame, double triggerXPosition)
{
    uint16_t i = ((uint16_t)(((double)samplesNumberOfOneFrame) * triggerXPosition)) + 5;
    return new Command(CMDCODE_PRE_TRIGGER_LENGTH,
                       (uint8_t)(i & 0xff),
                       (uint8_t)((i >> 8) & 0xff));
}

Command *CommandFactory::updatePostTriggerLength(uint16_t samplesNumberOfOneFrame, double triggerXPosition)
{
    uint16_t i = ((uint16_t)(((double)samplesNumberOfOneFrame) * (1 - triggerXPosition)));
    return new Command(CMDCODE_POST_TRIGGER_LENGTH,
                       (uint8_t)(i & 0xff),
                       (uint8_t)((i >> 8) & 0xff));
}