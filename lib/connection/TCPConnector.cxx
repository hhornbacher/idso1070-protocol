#include "connection/TCPConnector.h"

TCPConnector::TCPConnector(string host, int port) : host(host), port(port)
{
}

TCPConnector::~TCPConnector()
{
    if (socketHandle > 0)
        close(socketHandle);
}

void TCPConnector::start()
{
    if (socketHandle != -1)
        return;
    if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw ConnectionException("Cannot create socket");
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(host.c_str());

    if (connect(socketHandle,
                (struct sockaddr *)&serverAddress,
                sizeof(serverAddress)) != 0)
    {
        throw ConnectionException("Cannot connect to server");
    }

    if (fcntl(socketHandle, F_SETFL, fcntl(socketHandle, F_GETFL) | O_NONBLOCK) < 0)
    {
        throw ConnectionException("Cannot configure socket for non-blocking mode");
    }
    connected = true;
}

void TCPConnector::stop()
{
    if (socketHandle != -1)
        close(socketHandle);
    connected = false;
    socketHandle = -1;
}

ConnectorType TCPConnector::getType()
{
    return CONNECTOR_WIFI;
}

void TCPConnector::transmit(uint8_t *data, size_t length)
{
    if (socketHandle == -1)
        return;
    if (send(socketHandle, data, length, 0) < 0)
    {
        stop();
        throw ConnectionException("Connection lost");
    }
}

size_t TCPConnector::receive()
{
    if (!rawBuffer.full())
    {
        size_t size = RawBufferLength - rawBuffer.size();
        uint8_t tmp[size];
        if (socketHandle == -1)
            return;
        ssize_t result = recv(socketHandle, tmp, size, 0);
        if (result < 0)
        {
            stop();
            throw ConnectionException("Connection lost");
        }
        else if (result > 0)
        {
            for (ssize_t i = 0; i < result; i++)
                rawBuffer.push_back(tmp[i]);
        }
    }
    grabPacket();
    return rawBuffer.size();
}