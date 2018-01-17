#include "packets/Sample.h"

Sample::Sample(uint8_t *data) : Packet(data)
{
}

Sample::Sample(Response *response) : Packet(response->getHeader())
{
}

size_t Sample::getPayloadLength()
{
    return IDSO1070A::SamplesCountPerPacket;
}

void Sample::print()
{
    printf("[Sample]\n");
    printf("counter = %d\n", getCounter());
    printf("payloadLength = %ld\n", getPayloadLength());
    printf("[Header]\n");
    hexdump(getHeader(), HeaderSize);
    printf("[Payload]\n");
    hexdump(getPayload(), getPayloadLength());
    printf("\n\n");
}