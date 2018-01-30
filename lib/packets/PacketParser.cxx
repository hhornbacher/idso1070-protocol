#include "packets/PacketParser.h"

PacketParser::PacketParser(
    IDSO1070 &device,
    Sample::SampleBuffer &sampleBuffer1,
    Sample::SampleBuffer &sampleBuffer2) : device(device),
                                           sampleBuffer1(sampleBuffer1),
                                           sampleBuffer2(sampleBuffer2)
{
}

bool PacketParser::parse(Response *packet)
{
    switch (packet->getCommandType())
    {
    case TYPE_CONTROL:
        return parseAAResponse(packet);
    case TYPE_EEROM:
        return parseEEResponse(packet);
    case TYPE_FPGA:
        return parseFPGAResponse(packet);
    case TYPE_STATE:
        return parseStateResponse(packet);
    default:
        return false;
    }
}

void PacketParser::parse(Sample *packet)
{
    uint8_t head = packet->getPayload()[0];
    if (head & (1 << 5))
    {
        int i = head & 0x0f;
        if (device.getLittlePacketStatus() == i)
        {
            device.setLittlePacketStatus(device.getLittlePacketStatus() + 1);
            parseSamplePacket(packet, i);
            if (i == (device.getPacketsNumber() - 1))
            {
                device.setLittlePacketStatus(0);

                fixAdDiff();
                interpolateSamples();

                //             if (this.connector.isSendingCommands()) {
                //                 return;
                //             }

                if (head & (1 << 6))
                {
                    printf("\n\nTrigger compared\n\n");
                    // trigger compared
                }

                if (head & (1 << 4))
                {
                    printf("\n\nWave found\n\n");
                    // wave found
                }
                else
                {
                    // wave not found
                }

                return;
            }
            return;
        }
        device.setLittlePacketStatus(0);
        return;
    }
}

bool PacketParser::parseAAResponse(Response *packet)
{
    switch (packet->getCommandCode())
    {
    case 0x02:
        // Parse FPGA version
        char version[9];
        for (int i = 0; i < 8; i++)
        {
            version[i] = 0x30 + packet->getPayload()[6 + i];
        }
        version[8] = 0;
        device.setFPGAFirmwareVersion(version);
        return true;
    default:
        return false;
    }
}

bool PacketParser::parseEEResponse(Response *packet)
{
    if (packet->getCommandCode() == 0xaa)
    {
        switch (packet->getHeader()[5])
        {
        case 0x00:
            return parseEEROMPage00(packet);
        case 0x04:
            device.setFPGAAlert(packet->getPayload());
            return true;
        case 0x05:
            device.setUserName((char *)packet->getPayload());
            device.setProductName((char *)&packet->getPayload()[12]);
            return true;
        case 0x07:
            device.setDiffFixData(0, 0, packet->getPayload());
            return true;
        case 0x08:
            device.setDiffFixData(0, 100, packet->getPayload());
            return true;
        case 0x09:
            device.setDiffFixData(0, 200, packet->getPayload());
            return true;
        case 0x0a:
            device.setDiffFixData(1, 0, packet->getPayload());
            return true;
        case 0x0b:
            device.setDiffFixData(1, 100, packet->getPayload());
            return true;
        case 0x0c:
            device.setDiffFixData(1, 200, packet->getPayload());
            return true;
        default:
            return false;
        }
    }
}

bool PacketParser::parseFPGAResponse(Response *packet)
{
    switch (packet->getCommandCode())
    {
    case 0x02:
        return parseTriggerMode(packet);
    case 0x03:
        return parseRelay(packet);
    case 0x05:
        return parseTriggerSourceAndSlope(packet);
    case 0x06:
        return parseVoltsDiv125(packet);
    case 0x0b:
        return parseCh1ZeroLevel(packet);
    case 0x0c:
        return parseCh2ZeroLevel(packet);
    case 0x0d:
        return parseTriggerLevel(packet);
    case 0x12:
        return parseFreqDivLowBytes(packet);
    case 0x13:
        return parseFreqDivHighBytes(packet);
    case 0x15:
        return parseRamChannelSelection(packet);
    default:
        return false;
    }
}

bool PacketParser::parseStateResponse(Response *packet)
{
    switch (packet->getCommandCode())
    {
    case 0x03:
        device.setBatteryLevel(packet->getPayload()[0]);
        return true;
    case 0x04:
        char version[9];
        memcpy(version, packet->getPayload(), 8);
        version[8] = 0;
        device.setARMFirmwareVersion(version);
        return true;
    default:
        return false;
    }
}

bool PacketParser::parseFreqDivLowBytes(Response *packet)
{
    int i = ((packet->getHeader()[6] & 255) << 8) + (packet->getHeader()[5] & 255);
    if (device.getReceiveFreqDivStatus() == 0)
    {
        device.setReceiveFreqDivStatus(1);
        device.setFreqDiv(i);
        // resetRecvFreqStatusAfterDelay();
    }
    else if (device.getReceiveFreqDivStatus() == 2)
    {
        device.setReceiveFreqDivStatus(0);
        device.setFreqDiv(i + device.getFreqDiv());
    }
}

bool PacketParser::parseFreqDivHighBytes(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0xff) << 8) + (packet->getHeader()[5] & 0xff);

    if (device.getReceiveFreqDivStatus() == 0)
    {
        device.setReceiveFreqDivStatus(2);
        device.setFreqDiv(i << 16);
        // resetRecvFreqStatusAfterDelay();
    }
    else if (device.getReceiveFreqDivStatus() == 1)
    {
        device.setReceiveFreqDivStatus(0);
        device.setFreqDiv((i << 16) + device.getFreqDiv());
    }
}

bool PacketParser::parseRamChannelSelection(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x00:
        device.enableChannel(CHANNEL_1);
        device.enableChannel(CHANNEL_2);
        break;
    case 0x01:
        device.disableChannel(CHANNEL_1);
        device.disableChannel(CHANNEL_2);
        break;
    case 0x08:
        device.enableChannel(CHANNEL_1);
        device.disableChannel(CHANNEL_2);
        device.setSelectedChannel(CHANNEL_1);
        break;
    case 0x09:
        device.disableChannel(CHANNEL_1);
        device.enableChannel(CHANNEL_2);
        device.setSelectedChannel(CHANNEL_2);
        break;
    }
}

bool PacketParser::parseRelay(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x80:
        device.setChannelVoltageRL1(CHANNEL_1, 1.0);
        break;
    case 0xbf:
        device.setChannelVoltageRL1(CHANNEL_2, 0.1);
        // device.getChannel2().setVoltageRL1(0.1);
        break;
    case 0xfb:
        device.setChannelVoltageRL2(CHANNEL_2, 0.1);
        // device.getChannel2().setVoltageRL2(0.1);
        break;
    case 0xfd:
        device.setChannelVoltageRL2(CHANNEL_1, 0.1);
        // device.getChannel1().setVoltageRL2(0.1);
        break;
    case 0x02:
        device.setChannelVoltageRL2(CHANNEL_1, 1.0);
        // device.getChannel1().setVoltageRL2(1.0);
        break;
    case 0x04:
        device.setChannelVoltageRL2(CHANNEL_2, 1.0);
        // device.getChannel2().setVoltageRL2(1.0);
        break;
    case 0x40:
        device.setChannelVoltageRL1(CHANNEL_2, 1.0);
        // device.getChannel2().setVoltageRL1(1.0);
        break;
    case 0x7f:
        device.setChannelVoltageRL1(CHANNEL_1, 0.1);
        // device.getChannel1().setVoltageRL1(0.1);
        break;
    default:
        parseCoupling(packet);
        break;
    }
    // switch (packet->getHeader()[5])
    // {
    // case 0x80:
    // case 0x7f:
    // updateCh1VoltsDivStatusAfterReceivedRL1();
    //     return;
    // case 0xbf:
    // case 0x40:
    // updateCh2VoltsDivStatusAfterReceivedRL3();
    //     return;
    // case 0xfb:
    // case 0x04:
    // updateCh2VoltsDivStatusAfterReceivedRL4();
    //     return;
    // case 0xfd:
    // case 0x02:
    // updateCh1VoltsDivStatusAfterReceivedRL2();
    //     return;
    // default:
    //     return;
    // }
}

bool PacketParser::parseCh1ZeroLevel(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    int ordinal = (int)device.getChannelVerticalDiv(CHANNEL_1);
    i = (int)roundf(mapValue(i, (float)device.getChannelPWM(CHANNEL_1, ordinal, 0), (float)device.getChannelPWM(CHANNEL_1, ordinal, 1), 8.0f, 248.0f));
    device.setChannelVerticalPosition(CHANNEL_1, i);
}

bool PacketParser::parseCh2ZeroLevel(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    int ordinal = (int)device.getChannelVerticalDiv(CHANNEL_2);
    i = (int)roundf(mapValue(i, (float)device.getChannelPWM(CHANNEL_2, ordinal, 0), (float)device.getChannelPWM(CHANNEL_2, ordinal, 1), 8.0f, 248.0f));
    device.setChannelVerticalPosition(CHANNEL_2, i);
}

bool PacketParser::parseVoltsDiv125(Response *packet)
{
    switch (packet->getHeader()[5] & 3)
    {
    case 0:
        device.setChannelVoltage125(CHANNEL_1, 1.0);
        break;
    case 1:
        device.setChannelVoltage125(CHANNEL_1, 2.0);
        break;
    case 2:
        device.setChannelVoltage125(CHANNEL_1, 5.0);
        break;
    }
    // updateCh1VoltsDivStatusAfterReceived125();
    switch ((packet->getHeader()[5] >> 2) & 3)
    {
    case 0:
        device.setChannelVoltage125(CHANNEL_2, 1.0);
        break;
    case 1:
        device.setChannelVoltage125(CHANNEL_2, 2.0);
        break;
    case 2:
        device.setChannelVoltage125(CHANNEL_2, 5.0);
        break;
    }
    // updateCh2VoltsDivStatusAfterReceived125();
}

bool PacketParser::parseTriggerLevel(Response *packet)
{
    uint16_t i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    i = (uint16_t)roundf(mapValue(i, (float)device.getTriggerBottomPWM(), (float)device.getTriggerTopPWM(), 8.0f, 248.0f));
    device.setTriggerLevel(i);
}

bool PacketParser::parseTriggerSourceAndSlope(Response *packet)
{
    uint8_t i = packet->getHeader()[5] & 3;

    if (i == 0)
    {
        device.setTriggerChannel(TRIGCHAN_CH2);
    }
    else if (i == 1)
    {
        device.setTriggerChannel(TRIGCHAN_CH1);
    }
    else if (i == 2)
    {
        device.setTriggerChannel(TRIGCHAN_EXT);
    }
    if (packet->getHeader()[5] & (1 << 4))
    {
        device.setDeviceScopeMode(SCOMODE_ANALOG);
    }
    else
    {
        device.setDeviceScopeMode(SCOMODE_DIGITAL);
    }
    if (packet->getHeader()[5] & (1 << 7))
    {
        device.setTriggerSlope(TRIGSLOPE_RISING);
    }
    else
    {
        device.setTriggerSlope(TRIGSLOPE_FALLING);
    }
}

bool PacketParser::parseTriggerMode(Response *packet)
{
    device.setLittlePacketStatus(0);

    uint8_t b = packet->getHeader()[5];
    if (b & (1 << 0))
    {
        device.setDeviceCaptureMode(CAPMODE_ROLL);
    }
    else if (b & (1 << 3))
    {
        device.setDeviceCaptureMode(CAPMODE_SCAN);
    }
    else
    {
        device.setDeviceCaptureMode(CAPMODE_NORMAL);
    }
    if (b & (1 << 1))
    {
        device.setTriggerMode(TRIGMODE_AUTO);
    }
    else if (b & (1 << 2))
    {
        device.setTriggerMode(TRIGMODE_SINGLE);
    }
    else
    {
        device.setTriggerMode(TRIGMODE_NORMAL);
    }
}

bool PacketParser::parseCoupling(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0xef:
        device.setChannelCoupling(CHANNEL_1, COUPLING_DC);
        break;
    case 0xfe:
        device.setChannelCoupling(CHANNEL_2, COUPLING_DC);
        break;
    case 0xff:
        if (packet->getHeader()[6] == 0x01)
        {
            device.setChannelCoupling(CHANNEL_1, COUPLING_GND);
        }
        else if (packet->getHeader()[6] == 0x02)
        {
            device.setChannelCoupling(CHANNEL_2, COUPLING_GND);
        }
        break;
    case 0x01:
        device.setChannelCoupling(CHANNEL_1, COUPLING_AC);
        break;
    case 0x10:
        device.setChannelCoupling(CHANNEL_2, COUPLING_AC);
        break;
    }
}

bool PacketParser::parseEEROMPage00(Response *packet)
{
    device.setCaliLevel(packet->getPayload());
    uint16_t *iArr;
    int i = 0;
    int i2 = 0;

    int tmpA = 0;
    int tmpB = 0;
    while (tmpB < 9)
    {
        i = tmpA + 1;
        device.setChannelPWM(CHANNEL_1, packet->getPayload()[tmpA] & 255, tmpB, 0);
        iArr = device.getChannelPWM(CHANNEL_1, tmpB);
        int tmp = i + 1;
        iArr[0] = iArr[0] + ((packet->getPayload()[i] & 255) << 8);
        i = tmp + 1;
        device.setChannelPWM(CHANNEL_1, packet->getPayload()[tmp] & 255, tmpB, 1);
        iArr = device.getChannelPWM(CHANNEL_1, tmpB);
        i2 = i + 1;
        iArr[1] = ((packet->getPayload()[i] & 255) << 8) + iArr[1];
        tmpB++;
        tmpA = i2;
    }
    tmpB = tmpA;
    for (tmpA = 0; tmpA < 9; tmpA++)
    {
        i = tmpB + 1;
        device.setChannelPWM(CHANNEL_2, packet->getPayload()[tmpB] & 255, tmpA, 0);
        uint16_t *iArr2 = device.getChannelPWM(CHANNEL_2, tmpA);
        int tmp = i + 1;
        iArr2[0] = iArr2[0] + ((packet->getPayload()[i] & 255) << 8);
        i2 = tmp + 1;
        device.setChannelPWM(CHANNEL_2, packet->getPayload()[tmp] & 255, tmpA, 1);
        uint16_t *iArr3 = device.getChannelPWM(CHANNEL_2, tmpA);
        tmpB = i2 + 1;
        iArr3[1] = ((packet->getPayload()[i2] & 255) << 8) + iArr3[1];
    }
    i2 = tmpB + 1;
    device.setTriggerInnerPWM(0, packet->getPayload()[tmpB] & 255);
    iArr = device.getTriggerInnerPWM();
    i = i2 + 1;
    iArr[0] = iArr[0] + ((packet->getPayload()[i2] & 255) << 8);
    tmpB = i + 1;
    device.setTriggerInnerPWM(1, packet->getPayload()[i] & 255);
    iArr = device.getTriggerInnerPWM();
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.setTriggerInnerPWM(0, packet->getPayload()[i] & 255);
    iArr = device.getTriggerInnerPWM();
    i = tmpB + 1;
    iArr[0] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[0];
    tmpB = i + 1;
    device.setTriggerInnerPWM(1, packet->getPayload()[i] & 255);
    iArr = device.getTriggerInnerPWM();
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.setTriggerInnerPWM(2, packet->getPayload()[i] & 255);
    iArr = device.getTriggerInnerPWM();
    i = tmpB + 1;
    iArr[2] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[2];
    tmpB = i + 1;
    device.setTriggerInnerPWM(3, packet->getPayload()[i] & 255);
    iArr = device.getTriggerInnerPWM();
    i = tmpB + 1;
    iArr[3] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[3];
    if (device.getTriggerInnerPWM(2) < IDSO1070::SamplesCountPerPacket || device.getTriggerInnerPWM(2) > 4000)
    {
        device.setTriggerInnerPWM(2, device.getTriggerInnerPWM(0));
    }
    if (device.getTriggerInnerPWM(3) < IDSO1070::SamplesCountPerPacket || device.getTriggerInnerPWM(3) > 4000)
    {
        device.setTriggerInnerPWM(3, device.getTriggerInnerPWM(1));
    }
    return true;
}

void PacketParser::parseSamplePacket(Sample *packet, int index)
{
    if (device.getEnabledChannelsCount() == 2)
    {
        parseBothChannelsData(packet, index);
    }
    else if (device.isChannelEnabled(CHANNEL_1) && !device.isChannelEnabled(CHANNEL_2))
    {
        parseChannel1Data(packet, index);
    }
    else if (device.isChannelEnabled(CHANNEL_2) && !device.isChannelEnabled(CHANNEL_1))
    {
        parseChannel2Data(packet, index);
    }
}

void PacketParser::parseBothChannelsData(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * (IDSO1070::SamplesCountPerPacket / 2);

    while ((pos * 2) < IDSO1070::SamplesCountPerPacket)
    {
        if (device.getChannelCoupling(CHANNEL_1) == COUPLING_GND)
        {
            sampleBuffer1.push_back(device.getChannelVerticalPosition(CHANNEL_1));
        }
        else
        {
            sampleBuffer1.push_back((int16_t)(packet->getPayload()[1 + (pos * 2)] & 255));
        }
        if (device.getChannelCoupling(CHANNEL_2) == COUPLING_GND)
        {
            sampleBuffer2.push_back(device.getChannelVerticalPosition(CHANNEL_2));
        }
        else
        {
            sampleBuffer2.push_back((int16_t)(packet->getPayload()[1 + (pos * 2) + 1] & 255));
        }
        //     statisticCh1Max(sampleOffset + pos, this.channel1.getSamples()[sampleOffset + pos]);
        //     statisticCh1Min(sampleOffset + pos, this.channel1.getSamples()[sampleOffset + pos]);
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void PacketParser::parseChannel1Data(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * IDSO1070::SamplesCountPerPacket;
    while (pos < IDSO1070::SamplesCountPerPacket)
    {
        if (device.getChannelCoupling(CHANNEL_1) == COUPLING_GND)
        {
            sampleBuffer1.push_back(device.getChannelVerticalPosition(CHANNEL_1));
        }
        else
        {
            sampleBuffer1.push_back((int16_t)(packet->getPayload()[1 + pos] & 255));
        }
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void PacketParser::parseChannel2Data(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * IDSO1070::SamplesCountPerPacket;
    while (pos < IDSO1070::SamplesCountPerPacket)
    {
        if (device.getChannelCoupling(CHANNEL_2) == COUPLING_GND)
        {
            sampleBuffer2.push_back(device.getChannelVerticalPosition(CHANNEL_2));
        }
        else
        {
            sampleBuffer2.push_back((int16_t)(packet->getPayload()[1 + pos] & 255));
        }
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void PacketParser::fixAdDiff()
{
    if (device.getEnabledChannelsCount() == 1 && device.getDeviceTimeBase() <= HDIV_1uS)
    {
        fixCh1AdDiff();
        fixCh2AdDiff();
    }
}

void PacketParser::fixCh1AdDiff()
{
    //     if (device.getChannel1().isEnabled())
    //     {
    //         int i;
    //         short s;
    //         if ((EeromData.nFpgaAlert[16] == (short)0 ? 1 : 0) == 0)
    //         {
    //             for (i = 0; i < this.channel1.getLength(); i++)
    //             {
    //                 s = (short)(EeromData.adDiffFixData[0][this.channel1.getSamples()[i * 2]] + this.channel1.getSamples()[(i * 2) + 1]);
    //                 if (s > (short)255)
    //                 {
    //                     s = (short)255;
    //                 }
    //                 else if (s < (short)0)
    //                 {
    //                     s = (short)0;
    //                 }
    //                 this.channel1.getSamples()[(i * 2) + 1] = s;
    //             }
    //             return;
    //         }
    //         for (i = 0; i < this.channel1.getLength(); i++)
    //         {
    //             s = (short)(EeromData.adDiffFixData[0][this.channel1.getSamples()[i * 2]] + this.channel1.getSamples()[(i * 2) + 1]);
    //             if (s > (short)255)
    //             {
    //                 s = (short)255;
    //             }
    //             else if (s < (short)0)
    //             {
    //                 s = (short)0;
    //             }
    //             this.channel1.getSamples()[(i * 2) + 1] = this.channel1.getSamples()[i * 2];
    //             this.channel1.getSamples()[i * 2] = s;
    //         }
    //     }
}

void PacketParser::fixCh2AdDiff()
{
    //     int i;
    //     short s;
    //     if ((EeromData.nFpgaAlert[17] == (short)0 ? 1 : 0) == 0)
    //     {
    //         for (i = 0; i < this.channel2.getLength(); i++)
    //         {
    //             s = (short)(EeromData.adDiffFixData[1][this.channel2.getSamples()[(i * 2) + 1]] + this.channel2.getSamples()[i * 2]);
    //             if (s > (short)255)
    //             {
    //                 s = (short)255;
    //             }
    //             else if (s < (short)0)
    //             {
    //                 s = (short)0;
    //             }
    //             this.channel2.getSamples()[i * 2] = this.channel2.getSamples()[(i * 2) + 1];
    //             this.channel2.getSamples()[(i * 2) + 1] = s;
    //         }
    //         return;
    //     }
    //     for (i = 0; i < this.channel2.getLength(); i++)
    //     {
    //         s = (short)(EeromData.adDiffFixData[1][this.channel2.getSamples()[(i * 2) + 1]] + this.channel2.getSamples()[i * 2]);
    //         if (s > (short)255)
    //         {
    //             s = (short)255;
    //         }
    //         else if (s < (short)0)
    //         {
    //             s = (short)0;
    //         }
    //         this.channel2.getSamples()[i * 2] = s;
    //     }
}

void PacketParser::interpolateSamples()
{
    int i = 1;
    if (!(device.getDeviceTimeBase() == HDIV_1uS && device.getEnabledChannelsCount() == 2))
    {
        i = 0;
    }
    if (i != 0)
    {
        // IDSO1070Native.lerp_update(this.channel1.getInterpolatedSamples(), this.channel2.getInterpolatedSamples(), this.channel1.getSamples(), this.channel2.getSamples(), this.timeBase.ordinal(), channelStatus(), getTrigger().getTriggerChannel().ordinal(), (int)(getTrigger().getTriggerXPosition() * 100.0d), getTrigger().getTriggerLevel(), getTrigger().getTriggerSlope().ordinal(), IDSO1070::MemoryDepth, this.interpolateType.ordinal());
        if (device.isChannelEnabled(CHANNEL_1))
        {
            // System.arraycopy(this.channel1.getInterpolatedSamples(), 0, this.channel1.getSamples(), 0, IDSO1070::MemoryDepth);
        }
        if (device.isChannelEnabled(CHANNEL_2))
        {
            // System.arraycopy(this.channel2.getInterpolatedSamples(), 0, this.channel2.getSamples(), 0, IDSO1070::MemoryDepth);
        }
    }
}