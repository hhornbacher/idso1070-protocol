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

    bool onDone(uint8_t *cmdPayload, uint8_t *responsePayload, bool success)
    {
        printf("Done!");
        exit(0);
        return true;
    }

    bool onResponse(uint8_t *cmdPayload, uint8_t *responsePayload, bool success)
    {
        printf("Success: %d\n", success);
        return true;
    }

    int run()
    {

        protocol.start();
        protocol.sendCommand(
            cmdGen.readFPGAVersion(
                Command::bindHandler(&Main::onResponse, this)));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x00));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x04));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x05));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x07));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x08));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x09));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x0a));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x0b));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x0c));
        protocol.sendCommand(
            cmdGen.readEEROMPage(
                Command::bindHandler(&Main::onResponse, this),
                0x0c));
        protocol.sendCommands(cmdGen.init(Command::bindHandler(&Main::onDone, this)));

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