#include "Protocol.h"

Protocol::Protocol(Connector *connection, ResponseHandler *responseHandler) : cmdGen(&device), connection(connection), responseHandler(responseHandler)
{
    memset(&eeromData, 0, sizeof(EEROMData));
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

void Protocol::sendCommands(CommandQueue cmd)
{
    commandQueue.add(cmd);
}

void Protocol::sendCommands(Command *cmd)
{
    commandQueue.add(cmd);
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
            memcpy(eeromData.caliLevel, packet->getPayload(), 200);
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
        // Select Channel
        return;
    case 0x05:
        requestSuccess = true;
        parseTriggerSourceAndSlope(packet);
        return;
    case 0x06:
        requestSuccess = true;
        parseVoltsDiv125(packet);
        return;
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
        // RAM COUNT
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
    else if (device.receiveFreqDivStatus != 1 && device.receiveFreqDivStatus == 2)
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
        // ch1VoltageRL1 = 1.0;
        break;
    case 0xbf:
        // ch2VoltageRL3 = 0.1;
        break;
    case 0xfb:
        // ch2VoltageRL4 = 0.1;
        break;
    case 0xfd:
        // ch1VoltageRL2 = 0.1;
        break;
    case 0x02:
        // ch1VoltageRL2 = 1.0;
        break;
    case 0x04:
        // ch2VoltageRL4 = 1.0;
        break;
    case 0x40:
        // ch2VoltageRL3 = 1.0;
        break;
    case 0x7f:
        // ch1VoltageRL1 = 0.1;
        break;
    default:
        // parseCouplingReply(replyPacket);
        break;
    }
    switch (packet->getHeader()[5])
    {
    case 0x80:
    case 0x7f:
        // updateCh1VoltsDivStatusAfterReceivedRL1();
        return;
    case 0xbf:
    case 0x40:
        // updateCh2VoltsDivStatusAfterReceivedRL3();
        return;
    case 0xfb:
    case 0x04:
        // updateCh2VoltsDivStatusAfterReceivedRL4();
        return;
    case 0xfd:
    case 0x02:
        // updateCh1VoltsDivStatusAfterReceivedRL2();
        return;
    default:
        return;
    }
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
        // ch1Voltage125 = 1.0;
        break;
    case 1:
        // ch1Voltage125 = 2.0;
        break;
    case 2:
        // ch1Voltage125 = 5.0;
        break;
    }
    // updateCh1VoltsDivStatusAfterReceived125();
    switch ((packet->getHeader()[5] >> 2) & 3)
    {
    case 0:
        // ch2Voltage125 = 1.0;
        break;
    case 1:
        // ch2Voltage125 = 2.0;
        break;
    case 2:
        // ch2Voltage125 = 5.0;
        break;
    }
    // updateCh2VoltsDivStatusAfterReceived125();
}

void Protocol::parseTriggerLevel(ResponsePacket *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    i = (int)roundf(cmdGen.mapValue(i, (float)device.trigger.getBottomPWM(), (float)device.trigger.getTopPWM(), 8.0f, 248.0f));
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
    // connection->receive();
    // if (connection->getPacketBufferLength() == IDSO1070A_PACKET_SIZE)
    // {
    //     ResponsePacket packet(connection->getPacketBuffer());
    //     packet.print();
    //     parsePacket(&packet);
    //     if (expectedResponseCount > 0)
    //         expectedResponseCount--;

    //     // if (connection->getDoubleEEROMResponse() && packet.getType() == TYPE_EE)
    //     //     expectedResponseCount++;

    //     connection->clearPacketBuffer();
    // }
    connection->receive();
    if (connection->getPacketBufferLength() == IDSO1070A_PACKET_SIZE)
    {
        ResponsePacket packet(connection->getPacketBuffer());
        // packet.print();
        parsePacket(&packet);
        expectedResponseCount--;
        connection->clearPacketBuffer();
        if (expectedResponseCount == 0)
            state = STATE_DONE;
    }
}

void Protocol::transmit()
{
    // if (commandQueue.getSize() > 0 && expectedResponseCount == 0)
    // {
    //     Command *cmd = commandQueue.getNext();
    //     cmd->print();
    //     lastCommand = cmd;
    //     connection->transmit(cmd->getPayload(), 4);
    //     // expectedResponseCount++;
    //     expectedResponseCount = cmd->getResponseCount();
    //     delete cmd;
    // }

    Command *cmd = commandQueue.getNext();
    // cmd->print();
    lastCommand = cmd;
    connection->transmit(cmd->getPayload(), 4);
    expectedResponseCount = cmd->getResponseCount();
    state = STATE_RESPONSE;
}

void Protocol::process()
{
    switch (state)
    {
    case STATE_IDLE:
        if (commandQueue.getSize() > 0)
        {
            state = STATE_REQUEST;
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
            state = STATE_IDLE;
        break;
    }
}

CommandGenerator &Protocol::getCmdGen()
{
    return cmdGen;
}

void Protocol::resendLastCommand()
{
    commandQueue.addFront(lastCommand);
}

void Protocol::removeLastCommand()
{
    delete lastCommand;
}
