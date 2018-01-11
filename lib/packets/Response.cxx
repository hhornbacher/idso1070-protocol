#include "Response.h"

Response::Response(uint8_t *data) : Packet(data)
{
    memcpy(&rawPacket, data, PacketSize);
}
size_t Response::getPayloadLength()
{
    size_t pos = PayloadSize - 2;
    uint8_t filter = 0x5a;

    if (getType() == 0x57)
        filter = 0x00;
    while (getPayload()[pos] == filter)
    {
        pos--;
    }

    return pos + 1;
}

uint8_t Response::getCommandID()
{
    return rawPacket[4];
}

CommandType Response::getType()
{
    return (CommandType)rawPacket[3];
}

void Response::print()
{
    printf("[Response]\n");
    printf("counter = %d\n", getCounter());
    printf("commandID = %02x\n", getCommandID());
    printf("packetType = %d\n", getType());
    printf("payloadLength = %ld\n", getPayloadLength());
    printf("[Header]\n");
    hexdump(getHeader(), HeaderSize);
    printf("[Payload]\n");
    hexdump(getPayload(), getPayloadLength());
    printf("\n\n");
}