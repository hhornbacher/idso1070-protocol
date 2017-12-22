#include "Protocol.h"

#define BUF 1024
#define HEXDUMP_COLS 8

uint8_t packet03[] = {0xee, 0xaa, 0x00, 0x00};
uint8_t packet04[] = {0xee, 0xaa, 0x01, 0x00};
uint8_t packet05[] = {0xee, 0xaa, 0x04, 0x00};
uint8_t packet06[] = {0xee, 0xaa, 0x05, 0x00};
uint8_t packet07[] = {0xee, 0xaa, 0x06, 0x00};
uint8_t packet08[] = {0xee, 0xaa, 0x07, 0x00};
uint8_t packet09[] = {0xee, 0xaa, 0x08, 0x00};
uint8_t packet10[] = {0xee, 0xaa, 0x09, 0x00};
uint8_t packet11[] = {0xee, 0xaa, 0x0a, 0x00};
uint8_t packet12[] = {0xee, 0xaa, 0x0b, 0x00};
uint8_t packet13[] = {0xee, 0xaa, 0x0c, 0x00};
uint8_t packet14[] = {0xee, 0xaa, 0x0f, 0x00};

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

Command::Command(Commands cmd)
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
}

void Protocol::start()
{
    connection.start();
}

void Protocol::stop()
{
    connection.stop();
}

void Protocol::sendCommand(Command *cmd)
{
    connection.transmit(cmd->getPayload(), 4);
}

void Protocol::receivePackets(Packet *packets, size_t count)
{
    for (int i = 0; i < count; i++)
    {
        connection.clearPacketBuffer();
        while (connection.getPacketBufferLength() != IDSO10790A_PACKET_SIZE)
        {
            connection.receive();
        }
        memcpy(&packets[i], connection.getPacketBuffer(), IDSO10790A_PACKET_SIZE);
    }
    printf("Received %d packets\n\n", count);
}

void Protocol::process()
{
    Command cmd(RAM_CHANNEL_SELECTION);
    Packet ramChannelSelectionResponse;

    sendCommand(&cmd);
    receivePackets(&ramChannelSelectionResponse, 1);
    printf("Got packet1:\n");
    printf("Header:\n");
    hexdump(ramChannelSelectionResponse.header.bytes, 7);
    printf("Payload:\n");
    hexdump(ramChannelSelectionResponse.payload, IDSO10790A_PACKET_SIZE - 7);

    sendCommand(&cmd);
    receivePackets(&ramChannelSelectionResponse, 1);
    printf("Got packet2:\n");
    printf("Header:\n");
    hexdump(ramChannelSelectionResponse.header.bytes, 7);
    printf("Payload:\n");
    hexdump(ramChannelSelectionResponse.payload, IDSO10790A_PACKET_SIZE - 7);

    Command cmd03(packet03);
    Packet cmd03Response;

    sendCommand(&cmd03);
    receivePackets(&cmd03Response, 1);
    printf("Got packet3:\n");
    printf("Header:\n");
    hexdump(cmd03Response.header.bytes, 7);
    printf("Payload:\n");
    hexdump(cmd03Response.payload, IDSO10790A_PACKET_SIZE - 7);

    Command cmd04(packet04);
    Packet cmd04Response;

    sendCommand(&cmd04);
    receivePackets(&cmd04Response, 1);
    printf("Got packet4:\n");
    printf("Header:\n");
    hexdump(cmd04Response.header.bytes, 7);
    printf("Payload:\n");
    hexdump(cmd04Response.payload, IDSO10790A_PACKET_SIZE - 7);

    Command cmd05(packet05);
    Packet cmd05Response[2];

    sendCommand(&cmd05);
    receivePackets(cmd05Response, 2);
    printf("Got packet5:\n");
    printf("Header:\n");
    hexdump(cmd05Response[0].header.bytes, 7);
    printf("Payload:\n");
    hexdump(cmd05Response[0].payload, IDSO10790A_PACKET_SIZE - 7);
    printf("Header:\n");
    hexdump(cmd05Response[1].header.bytes, 7);
    printf("Payload:\n");
    hexdump(cmd05Response[1].payload, IDSO10790A_PACKET_SIZE - 7);

    // printf("Got packet5:\n");
    // hexdump(packetBuffer, 509 * 2);
    // request(packet06);
    // printf("Got packet6:\n");
    // hexdump(packetBuffer, 509);
    // request(packet07);
    // printf("Got packet7:\n");
    // hexdump(packetBuffer, 509);
    // request(packet08);
    // printf("Got packet8:\n");
    // hexdump(packetBuffer, 509);
    // request(packet09);
    // printf("Got packet9:\n");
    // hexdump(packetBuffer, 509);
    // request(packet10);
    // printf("Got packet10:\n");
    // hexdump(packetBuffer, 509);
    // request(packet11);
    // printf("Got packet:\n");
    // hexdump(packetBuffer, 509);
    // request(packet12);
    // printf("Got packet:\n");
    // hexdump(packetBuffer, 509);
    // request(packet13);
    // printf("Got packet:\n");
    // hexdump(packetBuffer, 509);
    // request(packet14);
    // printf("Got packet:\n");
    // hexdump(packetBuffer, 509);

    exit(0);
    // char *buffer[BUF];
    // int size;
    // switch (state)
    // {
    // case STATE1:
    //     break;
    // case STATE2:
    //     if (packetBufferOffset == 0)
    //     {
    //         printf("Got packet:\n");
    //         hexdump(packetBuffer, 21);
    //         memcpy(buffer, packet02, 4);
    //         send(socketHandle, buffer, 4, 0);
    //         size = recv(socketHandle, buffer, BUF - 1, 0);
    //         memccpy(packetBuffer, buffer, size, PACKETBUFFER_LENGTH);
    //         packetBufferOffset = size;
    //     }
    //     else
    //     {
    //         size = recv(socketHandle, buffer, BUF - 1, 0);
    //         memccpy(&packetBuffer[packetBufferOffset], buffer, size, PACKETBUFFER_LENGTH - packetBufferOffset);
    //         packetBufferOffset += size;
    //     }
    //     if (packetBufferOffset == 509)
    //     {
    //         state = STATE3;
    //         packetBufferOffset = 0;
    //     }
    //     break;
    // case STATE3:
    //     if (packetBufferOffset == 0)
    //     {
    //         printf("Got packet:\n");
    //         hexdump(packetBuffer, 21);
    //         memcpy(buffer, packet03, 4);
    //         send(socketHandle, buffer, 4, 0);
    //         size = recv(socketHandle, buffer, BUF - 1, 0);
    //         memccpy(packetBuffer, buffer, size, PACKETBUFFER_LENGTH);
    //         packetBufferOffset = size;
    //     }
    //     else
    //     {
    //         size = recv(socketHandle, buffer, BUF - 1, 0);
    //         memccpy(&packetBuffer[packetBufferOffset], buffer, size, PACKETBUFFER_LENGTH - packetBufferOffset);
    //         packetBufferOffset += size;
    //     }
    //     if (packetBufferOffset == 509)
    //     {
    //         state = STATE4;
    //         packetBufferOffset = 0;
    //     }
    //     break;
    // case STATE4:
    //     printf("Got packet:\n");
    //     hexdump(packetBuffer, 21);
    //     exit(0);
    //     break;
    // }
}
