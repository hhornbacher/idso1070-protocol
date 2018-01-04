#include "Protocol.h"

Protocol::Protocol(Connector *connection) : cmdGen(&device), connection(connection),
                                            readBatteryTimeout(10000), commandTimeout(200)
{
    memset(&eeromData, 0, sizeof(EEROMData));
    device.getChannel1().setVerticalPosition(188);
    // device.getChannel1().parseChVoltsDivStatus = PARSE_CHVOLTSDIV_S0;
    device.getChannel2().setVerticalPosition(68);
    // device.getChannel2().parseChVoltsDivStatus = PARSE_CHVOLTSDIV_S0;
    // this.autoMeasure.setAutoMeasureChannel(AutoMeasureChannel.CH1);
    // this.autoMeasure.setDvmChannel(DVMChannel.CH1);
    // this.channel1.setAttenuationFactor(AttenuationFactor.X1);
    // this.channel2.setAttenuationFactor(AttenuationFactor.X1);
}

Protocol::~Protocol()
{
}

void Protocol::start()
{
    connection->start();
}

void Protocol::stop()
{
    connection->stop();
}

IDSO1070A &Protocol::getDevice()
{
    return device;
}

EEROMData &Protocol::getEEROMData()
{
    return eeromData;
}

void Protocol::sendCommand(Commands cmd)
{
    switch (cmd)
    {
    case CMD_READ_FPGAVERSION_AND_EEPROM:
        commandQueue.add(CMD_READ_FPGA_VERSION);
        commandQueue.add(CMD_READ_EEROM_PAGE00);
        commandQueue.add(CMD_READ_EEROM_PAGE04);
        commandQueue.add(CMD_READ_EEROM_PAGE05);
        commandQueue.add(CMD_READ_EEROM_PAGE07);
        commandQueue.add(CMD_READ_EEROM_PAGE08);
        commandQueue.add(CMD_READ_EEROM_PAGE09);
        commandQueue.add(CMD_READ_EEROM_PAGE0A);
        commandQueue.add(CMD_READ_EEROM_PAGE0B);
        commandQueue.add(CMD_READ_EEROM_PAGE0C);
        break;
    case CMD_UPDATE_TRIGGER_SOURCE:
        commandQueue.add(CMD_UPDATE_TRIGGER_SOURCE_AND_SLOPE);
        commandQueue.add(CMD_UPDATE_TRIGGER_LEVEL);
        break;
    case CMD_UPDATE_TIME_BASE:
        commandQueue.add(CMD_UPDATE_SAMPLE_RATE);
        commandQueue.add(CMD_GET_FREQDIV_LOW_BYTES);
        commandQueue.add(CMD_GET_FREQDIV_HIGH_BYTES);
        commandQueue.add(CMD_PRE_TRIGGER);
        commandQueue.add(CMD_POST_TRIGGER);
        commandQueue.add(CMD_READ_RAM_COUNT);
        break;
    case CMD_INITIALIZE:
        commandQueue.add(CMD_SELECT_CHANNEL);
        commandQueue.add(CMD_SELECT_RAM_CHANNEL);
        commandQueue.add(CMD_READ_RAM_COUNT);
        commandQueue.add(CMD_UPDATE_CHANNEL_VOLTS125);
        commandQueue.add(CMD_RELAY1);
        commandQueue.add(CMD_RELAY2);
        commandQueue.add(CMD_RELAY3);
        commandQueue.add(CMD_RELAY4);
        commandQueue.add(CMD_CHANNEL1_LEVEL);
        commandQueue.add(CMD_CHANNEL2_LEVEL);
        commandQueue.add(CMD_UPDATE_SAMPLE_RATE);
        commandQueue.add(CMD_GET_FREQDIV_LOW_BYTES);
        commandQueue.add(CMD_GET_FREQDIV_HIGH_BYTES);
        commandQueue.add(CMD_PRE_TRIGGER);
        commandQueue.add(CMD_POST_TRIGGER);
        commandQueue.add(CMD_READ_RAM_COUNT);
        commandQueue.add(CMD_UPDATE_TRIGGER_SOURCE_AND_SLOPE);
        commandQueue.add(CMD_UPDATE_TRIGGER_MODE);
        commandQueue.add(CMD_UPDATE_TRIGGER_LEVEL);
        commandQueue.add(CMD_CHANNEL1_LEVEL);
        commandQueue.add(CMD_CHANNEL2_LEVEL);
        commandQueue.add(CMD_CHANNEL1_COUPLING);
        commandQueue.add(CMD_CHANNEL2_COUPLING);
        commandQueue.add(CMD_UPDATE_TRIGGER_MODE);
        commandQueue.add(CMD_START_SAMPLING);
        break;
    case CMD_VOLTAGE_DIV:
        commandQueue.add(CMD_UPDATE_CHANNEL_VOLTS125);
        commandQueue.add(CMD_RELAY1);
        commandQueue.add(CMD_RELAY2);
        commandQueue.add(CMD_RELAY3);
        commandQueue.add(CMD_RELAY4);
        commandQueue.add(CMD_CHANNEL1_LEVEL);
        commandQueue.add(CMD_CHANNEL2_LEVEL);
        break;
    case CMD_PULL_SAMPLES:
        commandQueue.add(CMD_UPDATE_TRIGGER_MODE);
        commandQueue.add(CMD_START_SAMPLING);
        break;
    case CMD_CHANNEL_STATUS:
        commandQueue.add(CMD_SELECT_CHANNEL);
        commandQueue.add(CMD_SELECT_RAM_CHANNEL);
        commandQueue.add(CMD_READ_RAM_COUNT);
        commandQueue.add(CMD_UPDATE_SAMPLE_RATE);
        commandQueue.add(CMD_GET_FREQDIV_LOW_BYTES);
        commandQueue.add(CMD_GET_FREQDIV_HIGH_BYTES);
        commandQueue.add(CMD_PRE_TRIGGER);
        commandQueue.add(CMD_POST_TRIGGER);
        commandQueue.add(CMD_READ_RAM_COUNT);
        break;
    case CMD_CHANNEL_STATUS_ONLY:
        commandQueue.add(CMD_SELECT_CHANNEL);
        commandQueue.add(CMD_SELECT_RAM_CHANNEL);
        commandQueue.add(CMD_READ_RAM_COUNT);
        break;
    case CMD_LEVELS:
        commandQueue.add(CMD_CHANNEL1_LEVEL);
        commandQueue.add(CMD_CHANNEL2_LEVEL);
        commandQueue.add(CMD_UPDATE_TRIGGER_LEVEL);
        break;
    case CMD_TRIGGER:
        commandQueue.add(CMD_UPDATE_TRIGGER_SOURCE_AND_SLOPE);
        commandQueue.add(CMD_UPDATE_TRIGGER_MODE);
        commandQueue.add(CMD_UPDATE_TRIGGER_LEVEL);
        break;
    case CMD_X_TRIGGER_POS:
        commandQueue.add(CMD_PRE_TRIGGER);
        commandQueue.add(CMD_POST_TRIGGER);
        break;
    case CMD_CHANNEL1_VOLTAGE_DIV:
        commandQueue.add(CMD_UPDATE_CHANNEL_VOLTS125);
        commandQueue.add(CMD_RELAY1);
        commandQueue.add(CMD_RELAY2);
        commandQueue.add(CMD_CHANNEL1_LEVEL);
        break;
    case CMD_CHANNEL2_VOLTAGE_DIV:
        commandQueue.add(CMD_UPDATE_CHANNEL_VOLTS125);
        commandQueue.add(CMD_RELAY3);
        commandQueue.add(CMD_RELAY4);
        commandQueue.add(CMD_CHANNEL2_LEVEL);
        break;
    default:
        commandQueue.add(cmd);
        break;
    }
}

void Protocol::parsePacket(ResponsePacket *packet)
{
    switch (packet->getType())
    {
    case TYPE_AA:
        parseAAResponse(packet);
        return;
    case TYPE_EE:
        parseEEResponse(packet);
        return;
    case TYPE_FPGA:
        parseFPGAResponse(packet);
        return;
    case TYPE_STATE:
        parseStateResponse(packet);
        return;
    default:
        requestSuccess = false;
        printf("Unknown response type: 0x%02x\n", (uint8_t)packet->getType());
        packet->print();
        return;
    }
}

void Protocol::parseAAResponse(ResponsePacket *packet)
{
    switch (packet->getHeader()[4])
    {
    case 0x04:
        requestSuccess = true;
        parseSampleData(packet);
        return;
    case 0x02:
        requestSuccess = true;
        printf("readFPGAVersion\n");
        return;
    default:
        requestSuccess = false;
        printf("Unknown AA response: 0x%02x\n", (uint8_t)packet->getHeader()[4]);
        packet->print();
        return;
    }
}

void Protocol::parseEEResponse(ResponsePacket *packet)
{
    if (packet->getHeader()[4] == 0xaa)
    {
        switch (packet->getHeader()[5])
        {
        case 0x00:
            requestSuccess = true;
            parseEEROMPage00(packet);
            return;
        case 0x04:
            requestSuccess = true;
            memcpy(eeromData.fpgaAlert, packet->getPayload(), 40);
            return;
        case 0x05:
            requestSuccess = true;
            memcpy(eeromData.userName, packet->getPayload(), 12);
            memcpy(eeromData.productName, &packet->getPayload()[12], 20);
            return;
        case 0x07:
            requestSuccess = true;
            memcpy(&eeromData.diffFixData[0][0], packet->getPayload(), 100);
            return;
        case 0x08:
            requestSuccess = true;
            memcpy(&eeromData.diffFixData[0][100], packet->getPayload(), 100);
            return;
        case 0x09:
            requestSuccess = true;
            memcpy(&eeromData.diffFixData[0][200], packet->getPayload(), 56);
            return;
        case 0x0a:
            requestSuccess = true;
            memcpy(&eeromData.diffFixData[1][0], packet->getPayload(), 100);
            return;
        case 0x0b:
            requestSuccess = true;
            memcpy(&eeromData.diffFixData[1][100], packet->getPayload(), 100);
            return;
        case 0x0c:
            requestSuccess = true;
            memcpy(&eeromData.diffFixData[1][200], packet->getPayload(), 56);
            // readFPGAVersionAndEEROMHasDone();

            // TEST
            // sendCommands(cmdGen.initialize(device));
            // sendCommands(cmdGen.updateTimeBase(device));
            // sendCommands(cmdGen.channelStatus(device));
            // sendCommands(cmdGen.updateTriggerSource(device));
            // sendCommands(cmdGen.readBatteryLevel(device));
            // sendCommands(cmdGen.levels(device));

            return;
        default:
            requestSuccess = false;
            printf("Unknown EEROM page: 0x%02x\n", (uint8_t)packet->getHeader()[5]);
            packet->print();
            return;
        }
    }
}

void Protocol::parseFPGAResponse(ResponsePacket *packet)
{
    switch (packet->getHeader()[4])
    {
    case 0x02:
        requestSuccess = true;
        parseStartCapture(packet);
        return;
    case 0x03:
        requestSuccess = true;
        parseRelay(packet);
        return;
    case 0x04:
        requestSuccess = true;
        // Select Channel  response
        return;
    case 0x05:
        requestSuccess = true;
        parseTriggerSourceAndSlope(packet);
        return;
    case 0x06:
        requestSuccess = true;
        parseVoltsDiv125(packet);
        return;
    case 0x07:
        requestSuccess = true;
        // pre trigger response
        return;
    case 0x08:
        requestSuccess = true;
        // post trigger response
        return;
    // case 0x0a:
    //     // parseTriggerLevel response ??
    //     requestSuccess = true;
    //     return;
    case 0x0b:
        requestSuccess = true;
        parseCh1ZeroLevel(packet);
        return;
    case 0x0c:
        requestSuccess = true;
        parseCh2ZeroLevel(packet);
        return;
    case 0x0d:
        requestSuccess = true;
        parseTriggerLevel(packet);
        return;
    case 0x11:
        requestSuccess = true;
        // sample rate response
        return;
    case 0x12:
        requestSuccess = true;
        parseFreqDivLowBytes(packet);
        return;
    case 0x13:
        requestSuccess = true;
        parseFreqDivHighBytes(packet);
        return;
    case 0x15:
        requestSuccess = true;
        parseRamChannelSelection(packet);
        return;
    case 0x16:
        requestSuccess = true;
        // RAM COUNT response
        return;
    default:
        requestSuccess = false;
        printf("Unknown FPGA response type: 0x%02x\n", (uint8_t)packet->getHeader()[4]);
        packet->print();
        return;
    }
}

void Protocol::parseStateResponse(ResponsePacket *packet)
{
    switch (packet->getHeader()[4])
    {
    case 0x03:
        requestSuccess = true;
        device.setBatteryLevel(packet->getPayload()[0]);
        break;
    case 0x04:
        requestSuccess = true;
        // memcpy(device.date, packet->getPayload(), 8);
        // device.date[8] = 0;
        break;
    default:
        requestSuccess = false;
        printf("Unknown state response type: 0x%02x\n", (uint8_t)packet->getHeader()[4]);
        packet->print();
        return;
    }
}

void Protocol::parseSampleData(ResponsePacket *packet)
{
    printf("parseSampleData\n");
}

void Protocol::parseFreqDivLowBytes(ResponsePacket *packet)
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
        syncTimeBaseFromFreqDiv();
    }
}

void Protocol::parseFreqDivHighBytes(ResponsePacket *packet)
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
        syncTimeBaseFromFreqDiv();
    }
}

void Protocol::parseRamChannelSelection(ResponsePacket *packet)
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

void Protocol::parseRelay(ResponsePacket *packet)
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

void Protocol::parseCh1ZeroLevel(ResponsePacket *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    int ordinal = (int)device.getChannel1().getVerticalDiv();
    i = (int)roundf(cmdGen.mapValue(i, (float)device.getChannel1().getPWM(ordinal, 0), (float)device.getChannel1().getPWM(ordinal, 1), 8.0f, 248.0f));
    device.getChannel1().setVerticalPosition(i);
}

void Protocol::parseCh2ZeroLevel(ResponsePacket *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    int ordinal = (int)device.getChannel2().getVerticalDiv();
    i = (int)roundf(cmdGen.mapValue(i, (float)device.getChannel2().getPWM(ordinal, 0), (float)device.getChannel2().getPWM(ordinal, 1), 8.0f, 248.0f));
    device.getChannel2().setVerticalPosition(i);
}

void Protocol::parseVoltsDiv125(ResponsePacket *packet)
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

void Protocol::parseTriggerLevel(ResponsePacket *packet)
{
    uint16_t i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    i = (uint16_t)roundf(cmdGen.mapValue(i, (float)device.getTrigger().getBottomPWM(), (float)device.getTrigger().getTopPWM(), 8.0f, 248.0f));
    device.getTrigger().setTriggerLevel(i);
}

void Protocol::parseTriggerSourceAndSlope(ResponsePacket *packet)
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

void Protocol::parseStartCapture(ResponsePacket *packet)
{
    // this.littlePacketStatus = 0;

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

void Protocol::parseCoupling(ResponsePacket *packet)
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

void Protocol::parseEEROMPage00(ResponsePacket *packet)
{
    memcpy(eeromData.caliLevel, packet->getPayload(), 200);
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
        device.getChannel1().setPWM(packet->getPayload()[tmpB] & 255, tmpA, 0);
        uint16_t *iArr2 = device.getChannel1().getPWM(tmpA);
        int tmp = i + 1;
        iArr2[0] = iArr2[0] + ((packet->getPayload()[i] & 255) << 8);
        i2 = tmp + 1;
        device.getChannel1().setPWM(packet->getPayload()[tmp] & 255, tmpA, 1);
        uint16_t *iArr3 = device.getChannel1().getPWM(tmpA);
        tmpB = i2 + 1;
        iArr3[1] = ((packet->getPayload()[i2] & 255) << 8) + iArr3[1];
    }
    i2 = tmpB + 1;
    device.getTrigger().setInnerTriggerPWM(0, packet->getPayload()[tmpB] & 255);
    iArr = device.getTrigger().getInnerTriggerPWM();
    i = i2 + 1;
    iArr[0] = iArr[0] + ((packet->getPayload()[i2] & 255) << 8);
    tmpB = i + 1;
    device.getTrigger().setInnerTriggerPWM(1, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerTriggerPWM();
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.getTrigger().setInnerTriggerPWM(0, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerTriggerPWM();
    i = tmpB + 1;
    iArr[0] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[0];
    tmpB = i + 1;
    device.getTrigger().setInnerTriggerPWM(1, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerTriggerPWM();
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.getTrigger().setInnerTriggerPWM(2, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerTriggerPWM();
    i = tmpB + 1;
    iArr[2] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[2];
    tmpB = i + 1;
    device.getTrigger().setInnerTriggerPWM(3, packet->getPayload()[i] & 255);
    iArr = device.getTrigger().getInnerTriggerPWM();
    i = tmpB + 1;
    iArr[3] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[3];
    if (device.getTrigger().getInnerTriggerPWM(2) < IDSO1070A_SAMPLES_COUNT_PER_PACKET || device.getTrigger().getInnerTriggerPWM(2) > 4000)
    {
        device.getTrigger().setInnerTriggerPWM(2, device.getTrigger().getInnerTriggerPWM(0));
    }
    if (device.getTrigger().getInnerTriggerPWM(3) < IDSO1070A_SAMPLES_COUNT_PER_PACKET || device.getTrigger().getInnerTriggerPWM(3) > 4000)
    {
        device.getTrigger().setInnerTriggerPWM(3, device.getTrigger().getInnerTriggerPWM(1));
    }
}

void Protocol::syncTimeBaseFromFreqDiv()
{
    TimeBase timebaseFromFreqDiv = device.getTimebaseFromFreqDiv();

    if (timebaseFromFreqDiv == HDIV_2uS)
    {
        timebaseFromFreqDiv = HDIV_1mS;
    }

    // setTimeBase(timebaseFromFreqDiv);
}

void Protocol::receive()
{
    connection->receive();
    if (connection->getPacketBufferLength() == IDSO1070A_PACKET_SIZE)
    {
        ResponsePacket packet(connection->getPacketBuffer());
        parsePacket(&packet);
        expectedResponseCount--;
        connection->clearPacketBuffer();
        if (expectedResponseCount == 0)
            changeState(STATE_DONE);
    }
}

void Protocol::transmit()
{
    if (commandTimeout.isTimedOut())
    {
        Commands command = commandQueue.getNext();

        lastCommand = command;
        Command *cmd = getCommand(command);

        connection->transmit(cmd->getPayload(), 4);
        if (cmd->getPayload()[0] == 0xee && !connection->isUsbConnection())
            expectedResponseCount = 2;
        expectedResponseCount = 1;
        delete cmd;
        changeState(STATE_RESPONSE);
        commandTimeout.reset();
    }
}

Command *Protocol::getCommand(Commands cmd)
{
    switch (cmd)
    {
    case CMD_START_SAMPLING:
        return cmdGen.startSampling();
    case CMD_READ_EEROM_PAGE00:
        return cmdGen.readEEROMPage(0x00);
    case CMD_READ_EEROM_PAGE04:
        return cmdGen.readEEROMPage(0x04);
    case CMD_READ_EEROM_PAGE05:
        return cmdGen.readEEROMPage(0x05);
    case CMD_READ_EEROM_PAGE07:
        return cmdGen.readEEROMPage(0x07);
    case CMD_READ_EEROM_PAGE08:
        return cmdGen.readEEROMPage(0x08);
    case CMD_READ_EEROM_PAGE09:
        return cmdGen.readEEROMPage(0x09);
    case CMD_READ_EEROM_PAGE0A:
        return cmdGen.readEEROMPage(0x0a);
    case CMD_READ_EEROM_PAGE0B:
        return cmdGen.readEEROMPage(0x0b);
    case CMD_READ_EEROM_PAGE0C:
        return cmdGen.readEEROMPage(0x0c);
    case CMD_SELECT_CHANNEL:
        return cmdGen.selectChannel();
    case CMD_SELECT_RAM_CHANNEL:
        return cmdGen.selectRAMChannel();
    case CMD_READ_FPGA_VERSION:
        return cmdGen.readFPGAVersion();
    case CMD_READ_BATTERY_LEVEL:
        return cmdGen.readBatteryLevel();
    case CMD_READ_RAM_COUNT:
        return cmdGen.readRamCount();
    case CMD_GET_FREQDIV_LOW_BYTES:
        return cmdGen.getFreqDivLowBytes();
    case CMD_GET_FREQDIV_HIGH_BYTES:
        return cmdGen.getFreqDivHighBytes();
    case CMD_UPDATE_SAMPLE_RATE:
        return cmdGen.updateSampleRate();
    case CMD_UPDATE_TRIGGER_SOURCE_AND_SLOPE:
        return cmdGen.updateTriggerSourceAndSlope();
    case CMD_UPDATE_TRIGGER_LEVEL:
        return cmdGen.updateTriggerLevel();
    case CMD_UPDATE_CHANNEL_VOLTS125:
        return cmdGen.updateChannelVolts125();
    case CMD_UPDATE_TRIGGER_MODE:
        return cmdGen.updateTriggerMode();
    case CMD_RELAY1:
        return cmdGen.relay1();
    case CMD_RELAY2:
        return cmdGen.relay2();
    case CMD_RELAY3:
        return cmdGen.relay3();
    case CMD_RELAY4:
        return cmdGen.relay4();
    case CMD_CHANNEL1_LEVEL:
        return cmdGen.channel1Level();
    case CMD_CHANNEL2_LEVEL:
        return cmdGen.channel2Level();
    case CMD_CHANNEL1_COUPLING:
        return cmdGen.channel1Coupling();
    case CMD_CHANNEL2_COUPLING:
        return cmdGen.channel2Coupling();
    case CMD_PRE_TRIGGER:
        return cmdGen.preTrigger();
    case CMD_POST_TRIGGER:
        return cmdGen.postTrigger();
    }
}

void Protocol::changeState(States state)
{
    this->state = state;
}

void Protocol::process()
{
    switch (state)
    {
    case STATE_IDLE:
        if (commandQueue.getSize() > 0)
        {
            changeState(STATE_REQUEST);
        }
        break;
    case STATE_REQUEST:
        transmit();
        break;
    case STATE_RESPONSE:
        receive();
        break;
    case STATE_DONE:
        // if (responseHandler->onResponse(lastCommand, requestSuccess))
        changeState(STATE_IDLE);
        break;
    }
    // if (readBatteryTimeout.isTimedOut())
    // {
    //     printf("Send read battery!\n");
    //     sendCommands(cmdGen.readBatteryLevel());
    //     readBatteryTimeout.reset();
    // }
}

void Protocol::resendLastCommand()
{
    commandQueue.addFront(lastCommand);
}

void Protocol::print()
{
    eeromData.print();
    device.print();
}