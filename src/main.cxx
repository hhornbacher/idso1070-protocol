#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include "Protocol.h"

void sigHandler(int sig);

#define CONNECTION usbConnection

class Main
{
  private:
    bool runProgram = true;
    const char *device = "/dev/ttyACM0";

    int resendCounter = 0;
    const char *serverIP = "192.168.1.1";
    const uint16_t serverPort = 8870;

    TCPConnector wifiConnection;
    USBConnector usbConnection;
    Protocol protocol;

  public:
    Main() : wifiConnection((char *)serverIP, serverPort), usbConnection((char *)device), protocol(&CONNECTION)
    {
        signal(SIGINT, sigHandler);
    }

    void stop()
    {
        runProgram = false;
    }

    int run()
    {

        protocol.start();
        // protocol.sendCommand(CMD_READ_FPGAVERSION_AND_EEPROM);
        // protocol.sendCommand(CMD_INITIALIZE);
        // protocol.sendCommand(CMD_PULL_SAMPLES);

        while (runProgram)
        {
            protocol.process();
        }
        return EXIT_SUCCESS;
    }
} program;

int main(int argc, char **argv)
{
    return program.run();
}

void sigHandler(int sig)
{
    if (sig == SIGINT)
    {
        program.stop();
        exit(0);
    }
}