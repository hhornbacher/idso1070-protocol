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
    memcpy(rawBuffer, data, Response::PacketSize);
    rawBufferLength = Response::PacketSize;
    pending = true;
}