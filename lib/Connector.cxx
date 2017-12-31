#include "Connector.h"

uint8_t *Connector::getPacketBuffer()
{
    return packetBuffer;
}

void Connector::clearPacketBuffer()
{
    packetBufferLength = 0;
}

size_t Connector::getPacketBufferLength()
{
    return packetBufferLength;
}

bool Connector::isUsbConnection()
{
    return usbConnection;
}