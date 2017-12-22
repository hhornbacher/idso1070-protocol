#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include "Protocol.h"

void sigHandler(int sig);

Protocol *connection;

const char *serverIP = "192.168.1.1";
const uint16_t serverPort = 8870;

int main(int argc, char **argv)
{
    signal(SIGINT, sigHandler);

    connection = new Protocol((char *)serverIP, serverPort);
    connection->start();

    while (true)
    {
        connection->process();
    }

    return EXIT_SUCCESS;
}

void sigHandler(int sig)
{
    if (sig == SIGINT)
    {
        connection->stop();
        exit(0);
    }
}