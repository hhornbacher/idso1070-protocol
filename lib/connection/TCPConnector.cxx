#include "connection/TCPConnector.h"

TCPConnector::TCPConnector(string host, int port) : host(host), port(port), socketHandle(-1)
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
        throw Exception("Cannot create socket");
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(host.c_str());

    int res = 0;
    if ((res = connect(socketHandle,
                       (struct sockaddr *)&serverAddress,
                       sizeof(serverAddress))) != 0)
    {
        throw Exception(strerror(res));
    }

    if ((res = fcntl(socketHandle, F_SETFL, fcntl(socketHandle, F_GETFL) | O_NONBLOCK)) < 0)
    {
        throw Exception("Unabled to put socket in non-blocking mode");
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
    int res = 0;
    if ((res = send(socketHandle, data, length, 0)) < 0)
    {
        stop();
        throw Exception(strerror(res));
        // throw Exception("Connection lost");
    }
}

void TCPConnector::receive()
{
    if (!rawBuffer.full())
    {
        size_t size = RawBufferLength - rawBuffer.size();
        uint8_t localBuffer[size];
        if (socketHandle == -1)
            return;
        ssize_t receivedSize = recv(socketHandle, localBuffer, size, 0);
        if (receivedSize < 0)
        {
            stop();
            throw Exception(strerror(receivedSize));
            // throw Exception("Connection lost");
        }
        else if (receivedSize > 0)
        {
            for (ssize_t i = 0; i < receivedSize; i++)
                rawBuffer.push_back(localBuffer[i]);
        }
    }
    grabPacket();
}