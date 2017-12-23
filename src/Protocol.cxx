#include "Protocol.h"

#define BUF 1024
#define HEXDUMP_COLS 16

void hexdump(uint8_t *data, size_t length)
{
    int rowCount = (length / HEXDUMP_COLS) + (length % HEXDUMP_COLS != 0 ? 1 : 0);
    for (int row = 0; row < rowCount; row++)
    {
        int colCount = row == (length / HEXDUMP_COLS) ? length % HEXDUMP_COLS : HEXDUMP_COLS;
        printf("0x%08x: ", row * HEXDUMP_COLS);
        for (int col = 0; col < colCount; col++)
        {
            unsigned int x = (unsigned int)data[(row * HEXDUMP_COLS) + col] & 0xff;
            printf("%02x ", x);
        }
        printf("\n");
    }
}

void printPacket(ResponsePacket *packet)
{
    printf("Got packet:\n");
    printf("Header:\n");
    hexdump(packet->getHeader(), 7);
    printf("Payload:\n");
    hexdump(packet->getPayload(), packet->getPayloadLength());
}

void printCommand(Command *cmd)
{
    printf("Sent command:\n");
    hexdump(cmd->getPayload(), 4);
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
    printf("Receive thread running!\n");
    while (self->receiving)
    {
        self->connection.receive();
        if (self->connection.getPacketBufferLength() == IDSO10790A_PACKET_SIZE)
        {
            ResponsePacket *packet = new ResponsePacket(self->connection.getPacketBuffer());
            self->packetQueue.push_back(packet);
            self->connection.clearPacketBuffer();
            printf("packetQueue size: %ld\n", self->packetQueue.size());
        }
    }
    printf("Receive thread stopped!\n");

    pthread_exit(0);
}

void *Protocol::transmit(void *arg)
{
    Protocol *self = (Protocol *)arg;
    printf("Transmit thread running!\n");
    while (self->transmitting)
    {
        if (self->commandQueue.size() > 0)
        {
            printf("commandQueue size: %ld\n", self->commandQueue.size());
            Command *cmd = self->commandQueue.front();
            self->connection.transmit(cmd->getPayload(), 4);
            printCommand(cmd);
            self->commandQueue.pop_front();
            delete cmd;
        }
    }
    printf("Transmit thread stopped!\n");

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

void Protocol::process()
{
    ResponsePacket *packet;
    CommandQueue commands;

    sendCommand(new Command(RAM_CHANNEL_SELECTION));
    waitForPackets(1);
    packet = packetQueue.front();
    printPacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommand(new Command(RAM_CHANNEL_SELECTION));
    waitForPackets(1);
    packet = packetQueue.front();
    printPacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommands(cmdGen.readEEROM());
    sleep(6);
    printf("%ld packets received!\n", packetQueue.size());
    for (PacketQueue::iterator i = packetQueue.begin(); i != packetQueue.end(); i++)
    {
        printPacket(*i);
        delete *i;
    }
    packetQueue.clear();

    sendCommand(cmdGen.readFPGAVersion());
    waitForPackets(1);
    packet = packetQueue.front();
    printPacket(packet);
    packetQueue.pop_front();
    delete packet;

    uint8_t cmd01[4] = {0x57, 0x04, 0x00, 0x00};
    sendCommand(new Command(cmd01));
    waitForPackets(1);
    packet = packetQueue.front();
    printPacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommand(new Command(SAMPLE_RATE));
    waitForPackets(1);
    packet = packetQueue.front();
    printPacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommand(new Command(FREQ_DIV_HIGH));
    waitForPackets(1);
    packet = packetQueue.front();
    printPacket(packet);
    packetQueue.pop_front();
    delete packet;

    sendCommands(cmdGen.getTimebase());
    sleep(6);
    printf("%ld packets received!\n", packetQueue.size());
    for (PacketQueue::iterator i = packetQueue.begin(); i != packetQueue.end(); i++)
    {
        printPacket(*i);
        delete *i;
    }
    packetQueue.clear();

    while (transmitting)
    {
        sendCommand(cmdGen.keepAlive());
        waitForPackets(1);
        packet = packetQueue.front();
        printPacket(packet);
        packetQueue.pop_front();
        delete packet;
        sleep(5);
    }

    exit(0);
}
