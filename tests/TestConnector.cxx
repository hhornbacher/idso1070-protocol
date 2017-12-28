#include "TestConnector.h"

TestConnector::TestConnector()
{
}
TestConnector::~TestConnector()
{
}

void TestConnector::transmit(uint8_t *data, size_t length)
{
}

size_t TestConnector::receive()
{
    if (pending)
    {
        pending = false;
        return IDSO1070A_PACKET_SIZE;
    }
    return 0;
}

void TestConnector::start()
{
}

void TestConnector::stop()
{
}

void TestConnector::fakePacket(uint8_t *data)
{
    memcpy(packetBuffer, data, IDSO1070A_PACKET_SIZE);
    packetBufferLength = IDSO1070A_PACKET_SIZE;
    pending = true;
}