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
    CommandGenerator cmdGen;

  public:
    Main() : wifiConnection((char *)serverIP, serverPort), usbConnection((char *)device), protocol(CONNECTION), cmdGen(protocol.getDevice())
    {
        signal(SIGINT, sigHandler);
    }

    void stop()
    {
        runProgram = false;
    }

    bool onEEROM00Response(uint8_t *cmdPayload, uint8_t *responsePayload, bool success)
    {
        printf("Success: %d\n", success);
        return true;
    }

    bool onFPGAResponse(uint8_t *cmdPayload, uint8_t *responsePayload, bool success)
    {
        printf("Success: %d\n", success);
        Command *cmd = cmdGen.readEEROMPage(0);
        cmd->setHandler(&Main::onEEROM00Response, this);
        protocol.sendCommand(cmd);
        return true;
    }

    int run()
    {

        protocol.start();
        Command *cmd = cmdGen.readFPGAVersion();
        cmd->setHandler(&Main::onFPGAResponse, this);
        protocol.sendCommand(cmd);
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