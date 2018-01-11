#include "ResponseParser.h"

ResponseParser::ResponseParser(IDSO1070A &device) : device(device)
{
}

bool ResponseParser::parse(Response *packet)
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

bool ResponseParser::parseAAResponse(Response *packet)
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

bool ResponseParser::parseEEResponse(Response *packet)
{
    if (packet->getCommandCode() == 0xaa)
    {
        switch (packet->getHeader()[5])
        {
        case 0x00:
            return parseEEROMPage00(packet);
        case 0x04:
            memcpy(device.getEEROMData().fpgaAlert, packet->getPayload(), 40);
            return true;
        case 0x05:
            memcpy(device.getEEROMData().userName, packet->getPayload(), 12);
            memcpy(device.getEEROMData().productName, &packet->getPayload()[12], 20);
            return true;
        case 0x07:
            memcpy(&device.getEEROMData().diffFixData[0][0], packet->getPayload(), 100);
            return true;
        case 0x08:
            memcpy(&device.getEEROMData().diffFixData[0][100], packet->getPayload(), 100);
            return true;
        case 0x09:
            memcpy(&device.getEEROMData().diffFixData[0][200], packet->getPayload(), 56);
            return true;
        case 0x0a:
            memcpy(&device.getEEROMData().diffFixData[1][0], packet->getPayload(), 100);
            return true;
        case 0x0b:
            memcpy(&device.getEEROMData().diffFixData[1][100], packet->getPayload(), 100);
            return true;
        case 0x0c:
            memcpy(&device.getEEROMData().diffFixData[1][200], packet->getPayload(), 56);
            // TBD: Set EEROM initialized = true
            return true;
        default:
            return false;
        }
    }
}

bool ResponseParser::parseFPGAResponse(Response *packet)
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

bool ResponseParser::parseStateResponse(Response *packet)
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

bool ResponseParser::parseFreqDivLowBytes(Response *packet)
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
        device.getTimebaseFromFreqDiv();
    }
}

bool ResponseParser::parseFreqDivHighBytes(Response *packet)
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
        device.getTimebaseFromFreqDiv();
    }
}

bool ResponseParser::parseRamChannelSelection(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x00:
        device.getChannel1().enable();
        device.getChannel2().enable();
        break;
    case 0x01:
        device.getChannel1().disable();
        device.getChannel2().disable();
        break;
    case 0x08:
        device.getChannel1().enable();
        device.getChannel2().disable();
        device.setSelectedChannel(device.getChannel1());
        break;
    case 0x09:
        device.getChannel1().disable();
        device.getChannel2().enable();
        device.setSelectedChannel(device.getChannel2());
        break;
    }
}

bool ResponseParser::parseRelay(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x80:
        device.getChannel1().setVoltageRL1(1.0);
        break;
    case 0xbf:
        device.getChannel2().setVoltageRL1(0.1);
        break;
    case 0xfb:
        device.getChannel2().setVoltageRL2(0.1);
        break;
    case 0xfd:
        device.getChannel1().setVoltageRL2(0.1);
        break;
    case 0x02:
        device.getChannel1().setVoltageRL2(1.0);
        break;
    case 0x04:
        device.getChannel2().setVoltageRL2(1.0);
        break;
    case 0x40:
        device.getChannel2().setVoltageRL1(1.0);
        break;
    case 0x7f:
        device.getChannel1().setVoltageRL1(0.1);
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

bool ResponseParser::parseCh1ZeroLevel(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    int ordinal = (int)device.getChannel1().getVerticalDiv();
    i = (int)roundf(mapValue(i, (float)device.getChannel1().getPWM(ordinal, 0), (float)device.getChannel1().getPWM(ordinal, 1), 8.0f, 248.0f));
    device.getChannel1().setVerticalPosition(i);
}

bool ResponseParser::parseCh2ZeroLevel(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    int ordinal = (int)device.getChannel2().getVerticalDiv();
    i = (int)roundf(mapValue(i, (float)device.getChannel2().getPWM(ordinal, 0), (float)device.getChannel2().getPWM(ordinal, 1), 8.0f, 248.0f));
    device.getChannel2().setVerticalPosition(i);
}

bool ResponseParser::parseVoltsDiv125(Response *packet)
{
    switch (packet->getHeader()[5] & 3)
    {
    case 0:
        device.getChannel1().setVoltage125(1.0);
        break;
    case 1:
        device.getChannel1().setVoltage125(2.0);
        break;
    case 2:
        device.getChannel1().setVoltage125(5.0);
        break;
    }
    // updateCh1VoltsDivStatusAfterReceived125();
    switch ((packet->getHeader()[5] >> 2) & 3)
    {
    case 0:
        device.getChannel2().setVoltage125(1.0);
        break;
    case 1:
        device.getChannel2().setVoltage125(2.0);
        break;
    case 2:
        device.getChannel2().setVoltage125(5.0);
        break;
    }
    // updateCh2VoltsDivStatusAfterReceived125();
}

bool ResponseParser::parseTriggerLevel(Response *packet)
{
    uint16_t i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    i = (uint16_t)roundf(mapValue(i, (float)device.getTrigger().getBottomPWM(), (float)device.getTrigger().getTopPWM(), 8.0f, 248.0f));
    device.getTrigger().setLevel(i);
}

bool ResponseParser::parseTriggerSourceAndSlope(Response *packet)
{
    uint8_t i = packet->getHeader()[5] & 3;

    if (i == 0)
    {
        device.getTrigger().setChannel(TRIGCHAN_CH2);
    }
    else if (i == 1)
    {
        device.getTrigger().setChannel(TRIGCHAN_CH1);
    }
    else if (i == 2)
    {
        device.getTrigger().setChannel(TRIGCHAN_EXT);
    }
    if (packet->getHeader()[5] & (1 << 4))
    {
        device.setScopeMode(SCOMODE_ANALOG);
    }
    else
    {
        device.setScopeMode(SCOMODE_DIGITAL);
    }
    if (packet->getHeader()[5] & (1 << 7))
    {
        device.getTrigger().setSlope(TRIGSLOPE_RISING);
    }
    else
    {
        device.getTrigger().setSlope(TRIGSLOPE_FALLING);
    }
}

bool ResponseParser::parseTriggerMode(Response *packet)
{
    device.setLittlePacketStatus(0);

    uint8_t b = packet->getHeader()[5];
    if (b & (1 << 0))
    {
        device.setCaptureMode(CAPMODE_ROLL);
    }
    else if (b & (1 << 3))
    {
        device.setCaptureMode(CAPMODE_SCAN);
    }
    else
    {
        device.setCaptureMode(CAPMODE_NORMAL);
    }
    if (b & (1 << 1))
    {
        device.getTrigger().setMode(TRIGMODE_AUTO);
    }
    else if (b & (1 << 2))
    {
        device.getTrigger().setMode(TRIGMODE_SINGLE);
    }
    else
    {
        device.getTrigger().setMode(TRIGMODE_NORMAL);
    }
}

bool ResponseParser::parseCoupling(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0xef:
        device.getChannel1().setCoupling(COUPLING_DC);
        break;
    case 0xfe:
        device.getChannel2().setCoupling(COUPLING_DC);
        break;
    case 0xff:
        if (packet->getHeader()[6] == 0x01)
        {
            device.getChannel1().setCoupling(COUPLING_GND);
        }
        else if (packet->getHeader()[6] == 0x02)
        {
            device.getChannel2().setCoupling(COUPLING_GND);
        }
        break;
    case 0x01:
        device.getChannel1().setCoupling(COUPLING_AC);
        break;
    case 0x10:
        device.getChannel2().setCoupling(COUPLING_AC);
        break;
    }
}

bool ResponseParser::parseEEROMPage00(Response *packet)
{
    memcpy(device.getEEROMData().caliLevel, packet->getPayload(), 200);
    uint16_t *iArr;
    int i = 0;
    int i2 = 0;

    int tmpA = 0;
    int tmpB = 0;
    while (tmpB < 9)
    {
        i = tmpA + 1;
        device.getChannel1().setPWM(packet->getPayload()[tmpA] & 255, tmpB, 0);
        iArr = device.getChannel1().getPWM(tmpB);
        int tmp = i + 1;
        iArr[0] = iArr[0] + ((packet->getPayload()[i] & 255) << 8);
        i = tmp + 1;
        device.getChannel1().setPWM(packet->getPayload()[tmp] & 255, tmpB, 1);
        iArr = device.getChannel1().getPWM(tmpB);
        i2 = i + 1;
        iArr[1] = ((packet->getPayload()[i] & 255) << 8) + iArr[1];
        tmpB++;
        tmpA = i2;
    }
    tmpB = tmpA;
    for (tmpA = 0; tmpA < 9; tmpA++)
    {
        i = tmpB + 1;
        device.getChannel2().setPWM(packet->getPayload()[tmpB] & 255, tmpA, 0);
        uint16_t *iArr2 = device.getChannel2().getPWM(tmpA);
        int tmp = i + 1;
        iArr2[0] = iArr2[0] + ((packet->getPayload()[i] & 255) << 8);
        i2 = tmp + 1;
        device.getChannel2().setPWM(packet->getPayload()[tmp] & 255, tmpA, 1);
        uint16_t *iArr3 = device.getChannel2().getPWM(tmpA);
        tmpB = i2 + 1;
        iArr3[1] = ((packet->getPayload()[i2] & 255) << 8) + iArr3[1];
    }
    i2 = tmpB + 1;
    device.getTrigger().setInnerPWM(0, packet->getPayload()[tmpB] & 255);
    iArr = device.getTrigger().getInnerPWM();
    i = i2 + 1;
    iArr[0] = iArr[0] + ((packet->getPayload()[i2] & 255) << 8);
    tmpB = i + 1;
    device.getTrigger().setInnerPWM(1, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerPWM();
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.getTrigger().setInnerPWM(0, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerPWM();
    i = tmpB + 1;
    iArr[0] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[0];
    tmpB = i + 1;
    device.getTrigger().setInnerPWM(1, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerPWM();
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.getTrigger().setInnerPWM(2, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerPWM();
    i = tmpB + 1;
    iArr[2] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[2];
    tmpB = i + 1;
    device.getTrigger().setInnerPWM(3, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerPWM();
    i = tmpB + 1;
    iArr[3] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[3];
    if (device.getTrigger().getInnerPWM(2) < IDSO1070A::SamplesCountPerPacket || device.getTrigger().getInnerPWM(2) > 4000)
    {
        device.getTrigger().setInnerPWM(2, device.getTrigger().getInnerPWM(0));
    }
    if (device.getTrigger().getInnerPWM(3) < IDSO1070A::SamplesCountPerPacket || device.getTrigger().getInnerPWM(3) > 4000)
    {
        device.getTrigger().setInnerPWM(3, device.getTrigger().getInnerPWM(1));
    }
    return true;
}