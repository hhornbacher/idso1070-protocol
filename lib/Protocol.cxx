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

uint8_t packet[] = {/* Packet 22 */
                    0xff, 0x01, 0xfa, 0xee, 0xaa, 0x05, 0x00, 0x49,
                    0x44, 0x53, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x49, 0x44, 0x53, 0x4f, 0x31,
                    0x30, 0x37, 0x30, 0x41, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36,
                    0x31, 0x32, 0x30, 0x30, 0x30, 0x35, 0x30, 0x33,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50,
                    0x31, 0x2e, 0x30, 0x37, 0x00, 0x44, 0x31, 0x2e,
                    0x30, 0x31, 0x00, 0x4d, 0x30, 0x30, 0x31, 0x43,
                    0x30, 0x30, 0x31, 0x32, 0x30, 0x31, 0x37, 0x30,
                    0x36, 0x32, 0x37, 0x32, 0x30, 0x31, 0x37, 0x30,
                    0x36, 0x32, 0x37, 0x54, 0x30, 0x30, 0x31, 0xff,
                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                    0xff, 0xff, 0xff, 0xa5, 0x42, 0xc4, 0x39, 0x7a,
                    0x3a, 0xc0, 0x4c, 0x1a, 0xa5, 0x59, 0x21, 0xbc,
                    0x71, 0x41, 0x60, 0xad, 0xdd, 0xbc, 0xfe, 0xd6,
                    0x82, 0x60, 0x0c, 0x00, 0x1e, 0xef, 0xc7, 0x70,
                    0x40, 0x4f, 0x6a, 0x6c, 0xff, 0x91, 0x08, 0x31,
                    0x4e, 0x24, 0xcb, 0xf7, 0x32, 0x60, 0xbe, 0x5a,
                    0x9e, 0x37, 0x40, 0x80, 0xc8, 0x8d, 0x4e, 0x9e,
                    0x2a, 0x86, 0x16, 0x34, 0x41, 0x74, 0x8d, 0xd4,
                    0x80, 0xa1, 0x82, 0xbd, 0x73, 0x10, 0x12, 0x41,
                    0xf6, 0xc4, 0x03, 0xcc, 0x17, 0x92, 0xc7, 0x1e,
                    0x5b, 0x30, 0x61, 0xa0, 0x14, 0x93, 0xcb, 0xac,
                    0xb3, 0x46, 0x2a, 0x9c, 0x3f, 0x6c, 0x15, 0xfe,
                    0x93, 0xe8, 0x69, 0xa2, 0x52, 0x35, 0xa8, 0x20,
                    0xd4, 0xef, 0x66, 0xb4, 0x43, 0x82, 0x7f, 0x28,
                    0xce, 0x8e, 0x25, 0x01, 0xd1, 0x2b, 0x9e, 0x7d,
                    0x4d, 0x4b, 0xb6, 0xbe, 0xa5, 0xcf, 0x50, 0xec,
                    0x2e, 0x93, 0xb3, 0xcb, 0xfe, 0xc6, 0xcc, 0xb8,
                    0xe9, 0x73, 0x65, 0xf6, 0x45, 0x93, 0x3e, 0xb2,
                    0xc4, 0x0d, 0x71, 0xed, 0x58, 0x3e, 0xe3, 0xfe,
                    0x01, 0x9f, 0x12, 0xec, 0x69, 0xc0, 0xec, 0x21,
                    0x52, 0xc2, 0x13, 0xa5, 0x59, 0xb3, 0xda, 0xd1,
                    0x28, 0x48, 0x94, 0x2c, 0x38, 0xe2, 0x30, 0x75,
                    0xf7, 0x20, 0x22, 0x6c, 0x9b, 0x74, 0xc5, 0x75,
                    0x83, 0x4d, 0x42, 0xe8, 0x3c, 0xe3, 0x09, 0x21,
                    0xe9, 0xe0, 0xb1, 0xa3, 0xba, 0xf9, 0x36, 0x3d,
                    0x6d, 0x49, 0x6a, 0x05, 0x3f, 0xd6, 0xf4, 0x7d,
                    0xe9, 0x09, 0x26, 0xfd, 0xed, 0x67, 0xa4, 0xa5,
                    0x84, 0x25, 0xd7, 0x18, 0x3c, 0x17, 0x8f, 0x7d,
                    0x6a, 0x43, 0x23, 0x93, 0xfe, 0x9e, 0x3a, 0x35,
                    0x16, 0x3a, 0x9b, 0x79, 0x3e, 0xcb, 0xc8, 0x7b,
                    0x16, 0x93, 0x4a, 0x26, 0x0f, 0xe1, 0x01, 0x53,
                    0xa5, 0x4b, 0xb2, 0xf6, 0x3d, 0xb5, 0x8a, 0x71,
                    0xc6, 0x53, 0x3e, 0x79, 0x7a, 0x23, 0x17, 0x77,
                    0x7a, 0xe7, 0x05, 0xe9, 0xd7, 0x6a, 0xd2, 0xe4,
                    0x10, 0x88, 0xdb, 0xd0, 0x0f, 0x77, 0x19, 0x79,
                    0xc6, 0x3f, 0x11, 0x8c, 0x6f, 0x9f, 0xe6, 0x94,
                    0x2a, 0x5a, 0xae, 0x6e, 0x69, 0xab, 0x9e, 0x85,
                    0x92, 0x44, 0x03, 0x84, 0x55, 0x7e, 0xed, 0x6b,
                    0x79, 0x10, 0x78, 0xc1, 0xab, 0xbe, 0x68, 0x85,
                    0xec, 0xa6, 0x3a, 0x1c, 0xd8, 0x25, 0x26, 0x84,
                    0xe4, 0xdf, 0xc6, 0xa5, 0x46, 0x8a, 0xce, 0xd1,
                    0xb7, 0xee, 0x82, 0xc2, 0xb6, 0x3d, 0x88, 0x22,
                    0x13, 0xc6, 0xeb, 0xdf, 0xf7, 0x08, 0x33, 0xd1,
                    0xe9, 0x48, 0x24, 0x2c, 0x51, 0xd6, 0x60, 0xb5,
                    0x00, 0xfb, 0x17, 0x26, 0xdd, 0x9e, 0x7a, 0x77,
                    0x58, 0x8e, 0x03, 0xf7, 0x36, 0xd9, 0xaa, 0xea,
                    0x76, 0xf6, 0x1c, 0x4b, 0x30, 0x83, 0x4b, 0xfd,
                    0x71, 0x3e, 0xa1, 0x1d, 0x5a, 0xc1, 0x37, 0x4c,
                    0x78, 0x70, 0x6f, 0x33, 0xf5, 0xf1, 0xe6, 0xde,
                    0x93, 0xfa, 0xc0, 0xfe, 0xbd, 0x0a, 0xbc, 0x42,
                    0xac, 0x92, 0x34, 0xd5, 0xce};

void Protocol::start()
{
    connection.start();
    printf("Creating threads...\n");
    pthread_create(&receiveThread, NULL, receive, this);
    pthread_create(&transmitThread, NULL, transmit, this);
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
        switch (packet->getHeader()[5])
        {
        case 0:
            parseEEROMPage00(packet);
            return;
        case 4:
            parseEEROMPage04(packet);
            return;
        case 5:
            parseEEROMPage05(packet);
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
            printf("Unknown EEROM page: 0x%02x\n", (uint8_t)packet->getHeader()[5]);
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
        printf("Unknown FPGA response type: 0x%02x\n", (uint8_t)packet->getHeader()[4]);
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
    eeromData.print();
    memcpy(eeromData.caliLevel, packet->getPayload(), 200);
    eeromData.print();
}

void Protocol::parseEEROMPage04(ResponsePacket *packet)
{
    printf("parseEEROMPage04\n");
    eeromData.print();
    memcpy(eeromData.fpgaAlert, packet->getPayload(), 40);
    eeromData.print();
}

void Protocol::parseEEROMPage05(ResponsePacket *packet)
{
    printf("parseEEROMPage05\n");
    eeromData.print();
    memcpy(eeromData.userName, packet->getPayload(), 12);
    memcpy(eeromData.productName, &packet->getPayload()[12], 20);
    eeromData.print();
}

void Protocol::parseEEROMPage07(ResponsePacket *packet)
{
    printf("parseEEROMPage07\n");
    memcpy(&eeromData.diffFixData[0][0], packet->getPayload(), 100);
}

void Protocol::parseEEROMPage08(ResponsePacket *packet)
{
    printf("parseEEROMPage08\n");
    memcpy(&eeromData.diffFixData[0][100], packet->getPayload(), 100);
}

void Protocol::parseEEROMPage09(ResponsePacket *packet)
{
    printf("parseEEROMPage09\n");
    memcpy(&eeromData.diffFixData[0][200], packet->getPayload(), 56);
}

void Protocol::parseEEROMPage0a(ResponsePacket *packet)
{
    printf("parseEEROMPage0a\n");
    memcpy(&eeromData.diffFixData[1][0], packet->getPayload(), 100);
}

void Protocol::parseEEROMPage0b(ResponsePacket *packet)
{
    printf("parseEEROMPage0b\n");
    memcpy(&eeromData.diffFixData[1][100], packet->getPayload(), 100);
}

void Protocol::parseEEROMPage0c(ResponsePacket *packet)
{
    printf("parseEEROMPage0c\n");
    memcpy(&eeromData.diffFixData[1][200], packet->getPayload(), 56);
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
