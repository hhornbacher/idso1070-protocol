#include "packets/Response.h"

Response::Response(uint8_t *data) : Packet(data)
{
    memcpy(&rawPacket, data, PacketSize);
}
size_t Response::getPayloadLength()
{
    size_t pos = PayloadSize - 2;
    uint8_t filter = 0x5a;

    if (getCommandType() == TYPE_STATE)
        filter = 0x00;
    while (getPayload()[pos] == filter)
    {
        pos--;
    }

    return pos + 1;
}

uint8_t Response::getCommandCode()
{
    return rawPacket[4];
}

CommandType Response::getCommandType()
{
    return (CommandType)rawPacket[3];
}