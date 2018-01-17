#include "StreamServer.h"

StreamServer::StreamServer(Channel &channel, int port) : channel(channel), port(port)
{
}

void StreamServer::start()
{
    const int y = 1;
    ssize_t size;
    sockaddr_in address;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(serverSocket, SOL_SOCKET,
               SO_REUSEADDR, &y, sizeof(int));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(serverSocket,
             (struct sockaddr *)&address,
             sizeof(address)) != 0)
    {
        printf("Cannot bind to port: %d\n", port);
        return;
    }
    listen(serverSocket, 5);
    if (fcntl(serverSocket, F_SETFL, fcntl(serverSocket, F_GETFL) | O_NONBLOCK) < 0)
    {
        printf("Error: Cannot set socket to non-blocking mode!\nExiting...\n\n");
    }
}

void StreamServer::stop()
{
    close(serverSocket);
}

void StreamServer::process()
{
    auto &buffer = channel.getSampleBuffer();
    if (clientSocket == -1)
    {
        socklen_t addrlen = sizeof(struct sockaddr_in);
        clientSocket = accept(serverSocket,
                              (struct sockaddr *)&clientAddress,
                              &addrlen);
    }
    if (clientSocket > 0 && buffer.size() > 0)
    {
        buffer.linearize();
        if (send(clientSocket, buffer.array_one().first, buffer.size() * sizeof(int16_t), 0) < 0)
        {
            close(clientSocket);
            clientSocket = -1;
        }
    }
}
