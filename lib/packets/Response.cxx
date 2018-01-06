#include "Response.h"

// size_t PacketSize = 509;
// size_t HeaderSize = 7;
// size_t PayloadSize = PacketSize - HeaderSize;

Response::Response(uint8_t *data)
{
    memcpy(&rawPacket, data, PacketSize);
}

uint8_t *Response::getHeader()
{
    return rawPacket.header;
}
uint8_t *Response::getPayload()
{
    return rawPacket.payload;
}
size_t Response::getPayloadLength()
{
    if (getType() == 0xaa && getCommandID() == 0x04)
        return IDSO1070A::SamplesCountPerPacket;
    size_t pos = PayloadSize - 2;
    uint8_t filter = 0x5a;

    if (getType() == 0x57)
        filter = 0x00;
    while (rawPacket.payload[pos] == filter)
    {
        pos--;
    }

    return pos + 1;
}

uint8_t Response::getCommandID()
{
    return rawPacket.header[4];
}

uint8_t Response::getCounter()
{
    return rawPacket.header[2];
}

ResponseType Response::getType()
{
    return (ResponseType)rawPacket.header[3];
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