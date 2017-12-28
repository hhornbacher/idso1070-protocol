#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include "USBConnector.h"
#include "Protocol.h"

void sigHandler(int sig);

const char *device = "/dev/ttyACM0";
// const char *serverIP = "192.168.1.1";
// const uint16_t serverPort = 8870;

USBConnector connection((char *)device);
Protocol protocol(&connection);

int main(int argc, char **argv)
{
    signal(SIGINT, sigHandler);

    protocol.start();

    while (true)
    {
        protocol.process();
        protocol.transmit();
        protocol.receive();
    }

    return EXIT_SUCCESS;
}

void sigHandler(int sig)
{
    if (sig == SIGINT)
    {
        protocol.stop();
        exit(0);
    }
}