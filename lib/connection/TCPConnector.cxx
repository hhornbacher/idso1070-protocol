#include "TCPConnector.h"

TCPConnector::TCPConnector(char *host, int port)
{
    usbConnection = false;
    if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Cannot create socket!\n");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(host);
}

TCPConnector::~TCPConnector()
{
    if (socketHandle > 0)
        close(socketHandle);
}

void TCPConnector::start()
{
    if (connect(socketHandle,
                (struct sockaddr *)&serverAddress,
                sizeof(serverAddress)) == 0)
        printf("Connected to server: %s\n",
               inet_ntoa(serverAddress.sin_addr));

    if (fcntl(socketHandle, F_SETFL, fcntl(socketHandle, F_GETFL) | O_NONBLOCK) < 0)
    {
        printf("Error: Cannot set socket to non-blocking mode!\nExiting...\n\n");
    }
}

void TCPConnector::stop()
{
    close(socketHandle);
    socketHandle = -1;
}

void TCPConnector::transmit(uint8_t *data, size_t length)
{
    send(socketHandle, data, length, 0);
}

size_t TCPConnector::receive()
{
    if (!rawBuffer.full())
    {
        size_t size = RawBufferLength - rawBuffer.size();
        uint8_t tmp[size];
        ssize_t result = recv(socketHandle, tmp, size, 0);
        if (result > 0)
        {
            for (ssize_t i = 0; i < result; i++)
                rawBuffer.push_back(tmp[i]);
        }
    }
    grabPacket();
    return rawBuffer.size();
}