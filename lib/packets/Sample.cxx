#include "packets/Sample.h"

Sample::Sample(uint8_t *data) : Packet(data)
{
}

Sample::Sample(Response *response) : Packet(response->getHeader())
{
}

size_t Sample::getPayloadLength()
{
    return IDSO1070::SamplesCountPerPacket;
}