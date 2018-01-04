#include "Response.h"

Response::Response(uint8_t *data)
{
    memcpy(&rawPacket, data, IDSO1070A_PACKET_SIZE);
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
    size_t pos = IDSO1070A_PACKET_PAYLOAD_SIZE - 2;
    uint8_t filter = 0;
    if (rawPacket.payload[IDSO1070A_PACKET_PAYLOAD_SIZE - 2] == 0x5a)
    {
        filter = 0x5a;
    }
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

ResponseType Response::getType()
{
    return (ResponseType)rawPacket.header[3];
}

void Response::print()
{
    printf("[Response]\n");
    printf("commandID = %02x\n", getCommandID());
    printf("packetType = %s\n", typeToString(getType()));
    printf("payloadLength = %ld\n", getPayloadLength());
    printf("[Header]\n");
    hexdump(getHeader(), IDSO1070A_PACKET_HEADER_SIZE);
    printf("[Payload]\n");
    hexdump(getPayload(), getPayloadLength());
    printf("\n\n");
}

const char *Response::typeToString(ResponseType type)
{
    switch (type)
    {
    case TYPE_AA:
        return "TYPE_AA";
    case TYPE_EE:
        return "TYPE_EE";
    case TYPE_FPGA:
        return "TYPE_FPGA";
    case TYPE_STATE:
        return "TYPE_STATE";
    default:
        return "UNKOWN";
    }
}
