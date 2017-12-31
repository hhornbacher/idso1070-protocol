#include "Protocol.h"

Protocol::Protocol(Connector *connection, ResponseHandler *responseHandler) : cmdGen(&device), connection(connection),
                                                                              responseHandler(responseHandler), readBatteryTimeout(10000),
                                                                              commandTimeout(200)
{
    memset(&eeromData, 0, sizeof(EEROMData));

    device.freqDiv = 0;
    device.trigger.setTriggerLevel(188);
    device.trigger.mode = TRIGMODE_AUTO;
    device.channel1.enabled = true;
    device.channel1.coupling = COUPLING_AC;
    device.channel1.verticalDiv = VDIV_1V;
    device.channel1.verticalPosition = 188;
    // device.channel1.parseChVoltsDivStatus = PARSE_CHVOLTSDIV_S0;
    device.channel2.enabled = true;
    device.channel2.coupling = COUPLING_AC;
    device.channel2.verticalDiv = VDIV_1V;
    device.channel2.verticalPosition = 68;
    // device.channel2.parseChVoltsDivStatus = PARSE_CHVOLTSDIV_S0;
    device.timeBase = HDIV_1mS;
    device.captureMode = CAPMODE_NORMAL;
    device.scopeMode = SCOMODE_ANALOG;
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
        device.batteryLevel = packet->getPayload()[0];
        break;
    case 0x04:
        requestSuccess = true;
        memcpy(device.date, packet->getPayload(), 8);
        device.date[8] = 0;
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
    if (device.receiveFreqDivStatus == 0)
    {
        device.receiveFreqDivStatus = 1;
        device.freqDiv = i;
        // resetRecvFreqStatusAfterDelay();
    }
    else if (device.receiveFreqDivStatus == 2)
    {
        device.receiveFreqDivStatus = 0;
        device.freqDiv = i + device.freqDiv;
        syncTimeBaseFromFreqDiv();
    }
}

void Protocol::parseFreqDivHighBytes(ResponsePacket *packet)
{
    int i = ((packet->getHeader()[6] & 0xff) << 8) + (packet->getHeader()[5] & 0xff);

    if (device.receiveFreqDivStatus == 0)
    {
        device.receiveFreqDivStatus = 2;
        device.freqDiv = i << 16;
        // resetRecvFreqStatusAfterDelay();
    }
    else if (device.receiveFreqDivStatus == 1)
    {
        device.receiveFreqDivStatus = 0;
        device.freqDiv = (i << 16) + device.freqDiv;
        syncTimeBaseFromFreqDiv();
    }
}

void Protocol::parseRamChannelSelection(ResponsePacket *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x00:
        device.channel1.enabled = true;
        device.channel2.enabled = true;
        break;
    case 0x01:
        device.channel1.enabled = false;
        device.channel2.enabled = false;
        break;
    case 0x08:
        device.channel1.enabled = true;
        device.channel2.enabled = false;
        device.selectedChannel = &device.channel1;
        break;
    case 0x09:
        device.channel1.enabled = false;
        device.channel2.enabled = true;
        device.selectedChannel = &device.channel2;
        break;
    }
}

void Protocol::parseRelay(ResponsePacket *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x80:
        device.ch1VoltageRL1 = 1.0;
        break;
    case 0xbf:
        device.ch2VoltageRL3 = 0.1;
        break;
    case 0xfb:
        device.ch2VoltageRL4 = 0.1;
        break;
    case 0xfd:
        device.ch1VoltageRL2 = 0.1;
        break;
    case 0x02:
        device.ch1VoltageRL2 = 1.0;
        break;
    case 0x04:
        device.ch2VoltageRL4 = 1.0;
        break;
    case 0x40:
        device.ch2VoltageRL3 = 1.0;
        break;
    case 0x7f:
        device.ch1VoltageRL1 = 0.1;
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
    int ordinal = (int)device.channel1.verticalDiv;
    i = (int)roundf(cmdGen.mapValue(i, (float)device.channel1.pwmArray[ordinal][0], (float)device.channel1.pwmArray[ordinal][1], 8.0f, 248.0f));
    device.channel1.setVerticalPosition(i);
}

void Protocol::parseCh2ZeroLevel(ResponsePacket *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    int ordinal = (int)device.channel2.verticalDiv;
    i = (int)roundf(cmdGen.mapValue(i, (float)device.channel2.pwmArray[ordinal][0], (float)device.channel2.pwmArray[ordinal][1], 8.0f, 248.0f));
    device.channel2.setVerticalPosition(i);
}

void Protocol::parseVoltsDiv125(ResponsePacket *packet)
{
    switch (packet->getHeader()[5] & 3)
    {
    case 0:
        device.ch1Voltage125 = 1.0;
        break;
    case 1:
        device.ch1Voltage125 = 2.0;
        break;
    case 2:
        device.ch1Voltage125 = 5.0;
        break;
    }
    // updateCh1VoltsDivStatusAfterReceived125();
    switch ((packet->getHeader()[5] >> 2) & 3)
    {
    case 0:
        device.ch2Voltage125 = 1.0;
        break;
    case 1:
        device.ch2Voltage125 = 2.0;
        break;
    case 2:
        device.ch2Voltage125 = 5.0;
        break;
    }
    // updateCh2VoltsDivStatusAfterReceived125();
}

void Protocol::parseTriggerLevel(ResponsePacket *packet)
{
    uint16_t i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    i = (uint16_t)roundf(cmdGen.mapValue(i, (float)device.trigger.getBottomPWM(), (float)device.trigger.getTopPWM(), 8.0f, 248.0f));
    device.trigger.setTriggerLevel(i);
}

void Protocol::parseTriggerSourceAndSlope(ResponsePacket *packet)
{
    uint8_t i = packet->getHeader()[5] & 3;

    if (i == 0)
    {
        device.trigger.channel = TRIGCHAN_CH2;
    }
    else if (i == 1)
    {
        device.trigger.channel = TRIGCHAN_CH1;
    }
    else if (i == 2)
    {
        device.trigger.channel = TRIGCHAN_EXT;
    }
    if (packet->getHeader()[5] & (1 << 4))
    {
        device.scopeMode = SCOMODE_ANALOG;
    }
    else
    {
        device.scopeMode = SCOMODE_DIGITAL;
    }
    if (packet->getHeader()[5] & (1 << 7))
    {
        device.trigger.slope = TRIGSLOPE_RISING;
    }
    else
    {
        device.trigger.slope = TRIGSLOPE_FALLING;
    }
}

void Protocol::parseStartCapture(ResponsePacket *packet)
{
    // this.littlePacketStatus = 0;

    uint8_t b = packet->getHeader()[5];
    if (b & (1 << 0))
    {
        device.captureMode = CAPMODE_ROLL;
    }
    else if (b & (1 << 3))
    {
        device.captureMode = CAPMODE_SCAN;
    }
    else
    {
        device.captureMode = CAPMODE_NORMAL;
    }
    if (b & (1 << 1))
    {
        device.trigger.mode = TRIGMODE_AUTO;
    }
    else if (b & (1 << 2))
    {
        device.trigger.mode = TRIGMODE_SINGLE;
    }
    else
    {
        device.trigger.mode = TRIGMODE_NORMAL;
    }
}

void Protocol::parseCoupling(ResponsePacket *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0xef:
        device.channel1.coupling = COUPLING_DC;
        break;
    case 0xfe:
        device.channel2.coupling = COUPLING_DC;
        break;
    case 0xff:
        if (packet->getHeader()[6] == 0x01)
        {
            device.channel1.coupling = COUPLING_GND;
        }
        else if (packet->getHeader()[6] == 0x02)
        {
            device.channel2.coupling = COUPLING_GND;
        }
        break;
    case 0x01:
        device.channel1.coupling = COUPLING_AC;
        break;
    case 0x10:
        device.channel2.coupling = COUPLING_AC;
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
        device.channel1.pwmArray[tmpB][0] = packet->getPayload()[tmpA] & 255;
        iArr = device.channel1.pwmArray[tmpB];
        int tmp = i + 1;
        iArr[0] = iArr[0] + ((packet->getPayload()[i] & 255) << 8);
        i = tmp + 1;
        device.channel1.pwmArray[tmpB][1] = packet->getPayload()[tmp] & 255;
        iArr = device.channel1.pwmArray[tmpB];
        i2 = i + 1;
        iArr[1] = ((packet->getPayload()[i] & 255) << 8) + iArr[1];
        tmpB++;
        tmpA = i2;
    }
    tmpB = tmpA;
    for (tmpA = 0; tmpA < 9; tmpA++)
    {
        i = tmpB + 1;
        device.channel2.pwmArray[tmpA][0] = packet->getPayload()[tmpB] & 255;
        uint16_t *iArr2 = device.channel2.pwmArray[tmpA];
        int tmp = i + 1;
        iArr2[0] = iArr2[0] + ((packet->getPayload()[i] & 255) << 8);
        i2 = tmp + 1;
        device.channel2.pwmArray[tmpA][1] = packet->getPayload()[tmp] & 255;
        uint16_t *iArr3 = device.channel2.pwmArray[tmpA];
        tmpB = i2 + 1;
        iArr3[1] = ((packet->getPayload()[i2] & 255) << 8) + iArr3[1];
    }
    i2 = tmpB + 1;
    device.trigger.innerTriggerPWM[0] = packet->getPayload()[tmpB] & 255;
    iArr = device.trigger.innerTriggerPWM;
    i = i2 + 1;
    iArr[0] = iArr[0] + ((packet->getPayload()[i2] & 255) << 8);
    tmpB = i + 1;
    device.trigger.innerTriggerPWM[1] = packet->getPayload()[i] & 255;
    iArr = device.trigger.innerTriggerPWM;
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.trigger.innerTriggerPWM[0] = packet->getPayload()[i] & 255;
    iArr = device.trigger.innerTriggerPWM;
    i = tmpB + 1;
    iArr[0] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[0];
    tmpB = i + 1;
    device.trigger.innerTriggerPWM[1] = packet->getPayload()[i] & 255;
    iArr = device.trigger.innerTriggerPWM;
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    device.trigger.innerTriggerPWM[2] = packet->getPayload()[i] & 255;
    iArr = device.trigger.innerTriggerPWM;
    i = tmpB + 1;
    iArr[2] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[2];
    tmpB = i + 1;
    device.trigger.innerTriggerPWM[3] = packet->getPayload()[i] & 255;
    iArr = device.trigger.innerTriggerPWM;
    i = tmpB + 1;
    iArr[3] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[3];
    if (device.trigger.innerTriggerPWM[2] < IDSO1070A_SAMPLES_COUNT_PER_PACKET || device.trigger.innerTriggerPWM[2] > 4000)
    {
        device.trigger.innerTriggerPWM[2] = device.trigger.innerTriggerPWM[0];
    }
    if (device.trigger.innerTriggerPWM[3] < IDSO1070A_SAMPLES_COUNT_PER_PACKET || device.trigger.innerTriggerPWM[3] > 4000)
    {
        device.trigger.innerTriggerPWM[3] = device.trigger.innerTriggerPWM[1];
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
        if (responseHandler->onResponse(lastCommand, requestSuccess))
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