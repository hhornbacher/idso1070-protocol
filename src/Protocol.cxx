#include "Protocol.h"

void printCommand(Command *cmd)
{
    printf("\e[38;5;22mSent command:\n");
    hexdump(cmd->getPayload(), 4);
    printf("\e[0m");
}

Command::Command(CommandCode cmd)
{
    payload[0] = 0x55;
    payload[1] = (uint8_t)cmd;
    payload[2] = 0;
    payload[3] = 0;
}

Command::Command(uint8_t *cmd)
{
    memcpy(payload, cmd, 4);
}

uint8_t *Command::getPayload()
{
    return payload;
}

Protocol::Protocol(char *host, int port) : connection(host, port)
{
}

Protocol::~Protocol()
{
    receiving = false;
    transmitting = false;
    pthread_join(receiveThread, NULL);
    pthread_join(transmitThread, NULL);
}

uint8_t packet[] = {/* Packet 8 */
                    0xff, 0x01, 0x02, 0x55, 0x15, 0x00, 0x00, 0x60,
                    0x04, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01,
                    0x06, 0x00, 0x05, 0x01, 0x08, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                    0x5a, 0x5a, 0x5a, 0x5a, 0xce};

void Protocol::start()
{
    // connection.start();
    // printf("Creating threads...\n");
    // pthread_create(&receiveThread, NULL, receive, this);
    // pthread_create(&transmitThread, NULL, transmit, this);
    parsePacket(new ResponsePacket(packet));
}

void Protocol::stop()
{
    connection.stop();
    receiving = false;
    transmitting = false;
    pthread_join(receiveThread, NULL);
    pthread_join(transmitThread, NULL);
}

void *Protocol::receive(void *arg)
{
    Protocol *self = (Protocol *)arg;
    while (self->receiving)
    {
        self->connection.receive();
        if (self->connection.getPacketBufferLength() == IDSO1070A_PACKET_SIZE)
        {
            ResponsePacket *packet = new ResponsePacket(self->connection.getPacketBuffer());
            self->packetQueue.push_back(packet);
            self->connection.clearPacketBuffer();
            printf("\e[38;5;73mpacketQueue size: %ld\e[0m\n", self->packetQueue.size());
        }
    }

    pthread_exit(0);
}

void *Protocol::transmit(void *arg)
{
    Protocol *self = (Protocol *)arg;
    while (self->transmitting)
    {
        if (self->commandQueue.size() > 0)
        {
            Command *cmd = self->commandQueue.front();
            self->connection.transmit(cmd->getPayload(), 4);
            printCommand(cmd);
            self->commandQueue.pop_front();
            delete cmd;
        }
        sleep(2);
    }

    pthread_exit(0);
}

void Protocol::sendCommands(CommandQueue cmd)
{
    for (CommandQueue::iterator i = cmd.begin(); i != cmd.end(); i++)
    {
        commandQueue.push_back(*i);
    }
}

void Protocol::sendCommand(Command *cmd)
{
    commandQueue.push_back(cmd);
}

void Protocol::waitForPackets(size_t count)
{
    while (count != packetQueue.size())
    {
    }
}

void Protocol::parsePacket(ResponsePacket *packet)
{
    printf("parsePacket\n");
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
    default:
        printf("Unknown response type: 0x%02x\n", (uint8_t)packet->getType());
        return;
    }
}

void Protocol::parseAAResponse(ResponsePacket *packet)
{
    printf("parseAAResponse\n");
    switch (packet->getHeader()[4])
    {
    case 0x04:
        parseSampleData(packet);
        return;
    default:
        return;
    }
}

void Protocol::parseEEResponse(ResponsePacket *packet)
{
    printf("parseEEResponse\n");
    if (packet->getHeader()[4] == 0xaa)
    {
        switch (packet->getPayload()[5])
        {
        case 0:
            parseEEROMPage00(packet);
            return;
        case 4:
            parseEEROMPage04(packet);
            return;
        case 7:
            parseEEROMPage07(packet);
            return;
        case 8:
            parseEEROMPage08(packet);
            return;
        case 9:
            parseEEROMPage09(packet);
            return;
        case 10:
            parseEEROMPage0a(packet);
            return;
        case 11:
            parseEEROMPage0b(packet);
            return;
        case 12:
            parseEEROMPage0c(packet);
            // readEEROMHasDone();
            return;
        default:
            return;
        }
    }
}

void Protocol::parseFPGAResponse(ResponsePacket *packet)
{
    printf("parseFPGAResponse\n");
    switch (packet->getHeader()[4])
    {
    case 0x02:
        parseStartCapture(packet);
        return;
    case 0x03:
        parseRelay(packet);
        return;
    case 0x05:
        parseTriggerSourceAndSlope(packet);
        return;
    case 0x06:
        parseVoltsDiv125(packet);
        return;
    case 0x0b:
        parseCh1ZeroLevel(packet);
        return;
    case 0x0c:
        parseCh2ZeroLevel(packet);
        return;
    case 0x0d:
        parseTriggerLevel(packet);
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
        return;
    }
}

void Protocol::parseSampleData(ResponsePacket *packet)
{
    printf("parseSampleData\n");
}

void Protocol::parseEEROMPage00(ResponsePacket *packet)
{
    printf("parseEEROMPage00\n");
}

void Protocol::parseEEROMPage04(ResponsePacket *packet)
{
    printf("parseEEROMPage04\n");
}

void Protocol::parseEEROMPage07(ResponsePacket *packet)
{
    printf("parseEEROMPage07\n");
}

void Protocol::parseEEROMPage08(ResponsePacket *packet)
{
    printf("parseEEROMPage08\n");
}

void Protocol::parseEEROMPage09(ResponsePacket *packet)
{
    printf("parseEEROMPage09\n");
}

void Protocol::parseEEROMPage0a(ResponsePacket *packet)
{
    printf("parseEEROMPage0a\n");
}

void Protocol::parseEEROMPage0b(ResponsePacket *packet)
{
    printf("parseEEROMPage0b\n");
}

void Protocol::parseEEROMPage0c(ResponsePacket *packet)
{
    printf("parseEEROMPage0c\n");
}

void Protocol::parseStartCapture(ResponsePacket *packet)
{
    printf("parseStartCapture\n");
}

void Protocol::parseRelay(ResponsePacket *packet)
{
    printf("parseRelay\n");
}

void Protocol::parseTriggerSourceAndSlope(ResponsePacket *packet)
{
    printf("parseTriggerSourceAndSlope\n");
}

void Protocol::parseVoltsDiv125(ResponsePacket *packet)
{
    printf("parseVoltsDiv125\n");
}

void Protocol::parseCh1ZeroLevel(ResponsePacket *packet)
{
    printf("parseCh1ZeroLevel\n");
}

void Protocol::parseCh2ZeroLevel(ResponsePacket *packet)
{
    printf("parseCh2ZeroLevel\n");
}

void Protocol::parseTriggerLevel(ResponsePacket *packet)
{
    printf("parseTriggerLevel\n");
}

void Protocol::parseFreqDivLowBytes(ResponsePacket *packet)
{
    printf("parseFreqDivLowBytes\n");
}

void Protocol::parseFreqDivHighBytes(ResponsePacket *packet)
{
    printf("parseFreqDivHighBytes\n");
}

void Protocol::parseRamChannelSelection(ResponsePacket *packet)
{
    printf("parseRamChannelSelection\n");
    device.print();
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
    device.print();
}

void Protocol::process()
{
    ResponsePacket *packet;
    CommandQueue commands;

    sendCommand(new Command(RAM_CHANNEL_SELECTION));
    waitForPackets(1);
    packet = packetQueue.front();
    parsePacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommand(new Command(RAM_CHANNEL_SELECTION));
    waitForPackets(1);
    packet = packetQueue.front();
    parsePacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommands(cmdGen.readEEROM());
    sleep(5);
    for (PacketQueue::iterator i = packetQueue.begin(); i != packetQueue.end(); i++)
    {
        parsePacket((*i));
        delete (*i);
    }
    packetQueue.clear();

    sendCommand(cmdGen.readFPGAVersion());
    waitForPackets(1);
    packet = packetQueue.front();
    parsePacket(packet);
    packetQueue.pop_front();
    delete packet;

    uint8_t cmd01[4] = {0x57, 0x04, 0x00, 0x00};
    sendCommand(new Command(cmd01));
    waitForPackets(1);
    packet = packetQueue.front();
    parsePacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommand(new Command(SAMPLE_RATE));
    waitForPackets(1);
    packet = packetQueue.front();
    parsePacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommand(new Command(FREQ_DIV_HIGH));
    waitForPackets(1);
    packet = packetQueue.front();
    parsePacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommands(cmdGen.getTimebase());
    sleep(6);
    for (PacketQueue::iterator i = packetQueue.begin(); i != packetQueue.end(); i++)
    {
        parsePacket((*i));
        delete (*i);
    }
    packetQueue.clear();

    while (transmitting)
    {
        sendCommand(cmdGen.keepAlive());
        waitForPackets(1);
        packet = packetQueue.front();
        parsePacket(packet);
        packetQueue.clear();
        delete packet;
        sleep(5);
    }

    exit(0);
}
