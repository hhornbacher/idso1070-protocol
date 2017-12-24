#include "ResponsePacket.h"

ResponsePacket::ResponsePacket(uint8_t *data)
{
    memcpy(&rawPacket, data, IDSO1070A_PACKET_SIZE);
}

uint8_t *ResponsePacket::getHeader()
{
    return rawPacket.header;
}
uint8_t *ResponsePacket::getPayload()
{
    return rawPacket.payload;
}
size_t ResponsePacket::getPayloadLength()
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

uint8_t ResponsePacket::getCommandID()
{
    return rawPacket.header[4];
}

PacketType ResponsePacket::getType()
{
    return (PacketType)rawPacket.header[3];
}

void ResponsePacket::print()
{
    printf("\e[38;5;75mGot packet:\n");
    printf("CommandID: %02x, Type: %s\n", getCommandID(), typeToString(getType()));
    printf("Payload:\n");
    hexdump(getPayload(), getPayloadLength());
    printf("\e[0m");
}

char *ResponsePacket::typeToString(PacketType type)
{
    switch (type)
    {
    case TYPE_AA:
        return "TYPE_AA";
    case TYPE_EE:
        return "TYPE_EE";
    case TYPE_FPGA:
        return "TYPE_FPGA";
    }
}
