#include "packets/Packet.h"

Packet::Packet(uint8_t *data)
{
    memcpy(&rawPacket, data, PacketSize);
}

uint8_t *Packet::getHeader()
{
    return rawPacket;
}

uint8_t *Packet::getPayload()
{
    return &rawPacket[HeaderSize];
}

uint8_t Packet::getCounter()
{
    return rawPacket[2];
}