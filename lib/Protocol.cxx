#include "Protocol.h"

Protocol::Protocol(char *host, int port) : connection(host, port)
{
}

Protocol::~Protocol()
{
    receiving = false;
    transmitting = false;
}

void Protocol::start()
{
    connection.start();
}

void Protocol::stop()
{
    connection.stop();
    receiving = false;
    transmitting = false;
}

void Protocol::receive()
{
    if (receiving)
    {
        connection.receive();
        if (connection.getPacketBufferLength() == IDSO1070A_PACKET_SIZE)
        {
            ResponsePacket *packet = new ResponsePacket(connection.getPacketBuffer());
            packetQueue.push_back(packet);
            connection.clearPacketBuffer();
            if (expectedResponseCount > 0)
                expectedResponseCount--;
            printf("\e[38;5;73mpacketQueue size: %ld\e[0m\n", packetQueue.size());
        }
    }
}

void Protocol::transmit()
{
    if (transmitting)
    {
        if (commandQueue.size() > 0 && expectedResponseCount == 0)
        {
            Command *cmd = commandQueue.front();
            connection.transmit(cmd->getPayload(), 4);
            expectedResponseCount = cmd->getResponseCount();
            commandQueue.pop_front();
            delete cmd;
        }
    }
}

void Protocol::sendCommands(CommandQueue cmd)
{
    for (CommandQueue::iterator i = cmd.begin(); i != cmd.end(); i++)
    {
        commandQueue.push_back(*i);
    }
}

void Protocol::sendCommands(Command *cmd)
{
    commandQueue.push_back(cmd);
}

ResponsePacket *Protocol::getCurrentPacket()
{
    return packetQueue.front();
}

void Protocol::removeCurrentPacket()
{
    delete getCurrentPacket();
    packetQueue.pop_front();
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
        parseSampleData(packet);
        return;
    case 0x02:
        // Probably some kind of ACK
        return;
    default:
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
            memcpy(eeromData.caliLevel, packet->getPayload(), 200);
            return;
        case 0x04:
            memcpy(eeromData.fpgaAlert, packet->getPayload(), 40);
            return;
        case 0x05:
            memcpy(eeromData.userName, packet->getPayload(), 12);
            memcpy(eeromData.productName, &packet->getPayload()[12], 20);
            return;
        case 0x07:
            memcpy(&eeromData.diffFixData[0][0], packet->getPayload(), 100);
            return;
        case 0x08:
            memcpy(&eeromData.diffFixData[0][100], packet->getPayload(), 100);
            return;
        case 0x09:
            memcpy(&eeromData.diffFixData[0][200], packet->getPayload(), 56);
            return;
        case 0x0a:
            memcpy(&eeromData.diffFixData[1][0], packet->getPayload(), 100);
            return;
        case 0x0b:
            memcpy(&eeromData.diffFixData[1][100], packet->getPayload(), 100);
            return;
        case 0x0c:
            memcpy(&eeromData.diffFixData[1][200], packet->getPayload(), 56);
            // readEEROMHasDone();
            return;
        default:
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
        printf("parseStartCapture\n");
        return;
    case 0x03:
        printf("parseRelay\n");
        return;
    case 0x05:
        printf("parseTriggerSourceAndSlope\n");
        return;
    case 0x06:
        printf("parseVoltsDiv125\n");
        return;
    case 0x0b:
        printf("parseCh1ZeroLevel\n");
        return;
    case 0x0c:
        printf("parseCh2ZeroLevel\n");
        return;
    case 0x0d:
        printf("parseTriggerLevel\n");
        return;
    case 0x12:
        parseFreqDivLowBytes(packet);
        return;
    case 0x13:
        parseFreqDivHighBytes(packet);
        return;
    case 0x15:
        parseRamChannelSelection(packet);
        return;
    default:
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
        device.batteryLevel = packet->getPayload()[0];
        break;
    case 0x04:
        memcpy(device.date, packet->getPayload(), 8);
        device.date[8] = 0;
        break;
    default:
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
        device.channel1 = true;
        device.channel2 = true;
        break;
    case 0x01:
        device.channel1 = false;
        device.channel2 = false;
        break;
    case 0x08:
        device.channel1 = true;
        device.channel2 = false;
        break;
    case 0x09:
        device.channel1 = false;
        device.channel2 = true;
        break;
    }
}

TimeBase Protocol::getTimebaseFromFreqDiv(uint32_t i)
{
    switch (i)
    {
    case 0:
        return HDIV_2uS;
    case 1:
        return HDIV_5uS;
    case 4:
        return HDIV_10uS;
    case 9:
        return HDIV_20uS;
    case 24:
        return HDIV_50uS;
    case 49:
        return HDIV_100uS;
    case 99:
        return HDIV_200uS;
    case 249:
        return HDIV_500uS;
    case 499:
        return HDIV_1mS;
    case 999:
        return HDIV_2mS;
    case 2499:
        return HDIV_5mS;
    case 4999:
        return HDIV_10mS;
    case 9999:
        return HDIV_20mS;
    case 24999:
        return HDIV_50mS;
    case 49999:
        return HDIV_100mS;
    case 99999:
        return HDIV_200mS;
    case 249999:
        return HDIV_500mS;
    case 499999:
        return HDIV_1S;
    case 999999:
        return HDIV_2S;
    case 2499999:
        return HDIV_5S;
    case 4999999:
        return HDIV_10S;
    case 9999999:
        return HDIV_20S;
    case 24999999:
        return HDIV_50S;
    case 49999999:
        return HDIV_100S;
    case 99999999:
        return HDIV_200S;
    case 249999999:
        return HDIV_500S;
    default:
        return HDIV_1mS;
    }
}

void Protocol::syncTimeBaseFromFreqDiv()
{
    TimeBase timebaseFromFreqDiv = getTimebaseFromFreqDiv(device.freqDiv);

    if (timebaseFromFreqDiv == HDIV_2uS)
    {
        timebaseFromFreqDiv = HDIV_1mS;
    }

    // setTimeBase(timebaseFromFreqDiv);
}

void Protocol::process()
{
    switch (state)
    {
    case STATE_IDLE:
        sendCommands(new Command(RAM_CHANNEL_SELECTION));
        state = STATE_RAM_CHANNEL_SELECTION;
        break;
    case STATE_RAM_CHANNEL_SELECTION:
        if (packetQueue.size() == 1)
        {
            parsePacket(getCurrentPacket());
            removeCurrentPacket();
            sendCommands(cmdGen.readEEROM());
            state = STATE_READ_EEROM;
        }
        break;
    case STATE_READ_EEROM:
        if (packetQueue.size() == 19)
        {
            while (packetQueue.size() > 0)
            {
                parsePacket(getCurrentPacket());
                removeCurrentPacket();
            }
            sendCommands(new Command(SAMPLE_RATE));
            state = STATE_SAMPLE_RATE;
        }
        break;
    case STATE_SAMPLE_RATE:
        if (packetQueue.size() == 1)
        {
            parsePacket(getCurrentPacket());
            removeCurrentPacket();
            sendCommands(new Command(FREQ_DIV_HIGH));
            state = STATE_FREQ_DIV_HIGH;
        }
        break;
    case STATE_FREQ_DIV_HIGH:
        if (packetQueue.size() == 1)
        {
            parsePacket(getCurrentPacket());
            removeCurrentPacket();
            sendCommands(new Command(FREQ_DIV_LOW));
            state = STATE_FREQ_DIV_LOW;
        }
        break;
    case STATE_FREQ_DIV_LOW:
        if (packetQueue.size() == 1)
        {
            parsePacket(getCurrentPacket());
            removeCurrentPacket();
            sendCommands(cmdGen.getTimebase());
            state = STATE_GET_TIMEBASE;
        }
        break;
    case STATE_GET_TIMEBASE:
        if (packetQueue.size() == 3)
        {
            while (packetQueue.size() > 0)
            {
                parsePacket(getCurrentPacket());
                removeCurrentPacket();
            }
            sendCommands(cmdGen.getTriggerSource());
            state = STATE_GET_TRIGGER_SOURCE;
        }
        break;
    case STATE_GET_TRIGGER_SOURCE:
        if (packetQueue.size() == 2)
        {
            while (packetQueue.size() > 0)
            {
                parsePacket(getCurrentPacket());
                removeCurrentPacket();
            }
            sendCommands(cmdGen.readBatteryLevel());
            device.print();
            eeromData.print();
            state = STATE_DONE;
        }
        break;
    case STATE_DONE:
        if (packetQueue.size() == 1)
        {
            sendCommands(cmdGen.readBatteryLevel());
            parsePacket(getCurrentPacket());
            removeCurrentPacket();
            sleep(1);
        }
        break;
    }
}
