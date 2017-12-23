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
    size_t pos = IDSO10790A_PACKET_PAYLOAD_SIZE - 2;
    uint8_t filter = 0;
    if(rawPacket.payload[ IDSO10790A_PACKET_PAYLOAD_SIZE - 2]==0x5a)
    {    
        filter=0x5a;
    }
    while (rawPacket.payload[pos] == filter)
    {
        pos--;
    }

    return pos + 1;
}