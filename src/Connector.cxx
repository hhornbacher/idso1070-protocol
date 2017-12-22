#include "Connector.h"

Connector::Connector(char *host, int port)
{

    if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Cannot create socket!\n");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(host);
}

Connector::~Connector()
{
    if (socketHandle > 0)
        close(socketHandle);
}

void Connector::start()
{
    if (connect(socketHandle,
                (struct sockaddr *)&serverAddress,
                sizeof(serverAddress)) == 0)
        printf("Verbindung mit dem Server (%s) hergestellt\n",
               inet_ntoa(serverAddress.sin_addr));
}

void Connector::stop()
{
    close(socketHandle);
    socketHandle = -1;
}

void Connector::transmit(uint8_t *data, size_t length)
{
    send(socketHandle, data, length, 0);
}

size_t Connector::receive()
{
    if (packetBufferLength < PACKETBUFFER_LENGTH)
        packetBufferLength += recv(socketHandle, &packetBuffer[packetBufferLength], PACKETBUFFER_LENGTH - packetBufferLength, 0);
    return packetBufferLength;
}

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