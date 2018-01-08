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
        return Response::PacketSize;
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
    for (ssize_t i = 0; i < Response::PacketSize; i++)
        rawBuffer.push_back(data[i]);
}