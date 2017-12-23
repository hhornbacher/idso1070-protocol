#include "ResponsePacket.h"

ResponsePacket::ResponsePacket(uint8_t *data)
{
    memcpy(&rawPacket, data, IDSO10790A_PACKET_SIZE);
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
    uint8_t *ptrOffset = (uint8_t *)memchr(rawPacket.payload, 0x5a, IDSO10790A_PACKET_SIZE);
    return (size_t)(rawPacket.payload - ptrOffset);
}