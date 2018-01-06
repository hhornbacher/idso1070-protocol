#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include "Protocol.h"

void sigHandler(int sig);

#define CONNECTION wifiConnection

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
    CommandFactory cmdFactory;
    ResponseParser parser;

  public:
    Protocol protocol;
    Main() : wifiConnection((char *)serverIP, serverPort),
             usbConnection((char *)device),
             protocol(CONNECTION), cmdFactory(protocol.getDevice()),
             parser(protocol.getDevice())
    {
        signal(SIGINT, sigHandler);
    }

    void stop()
    {
        runProgram = false;
    }

    void onSample(Response *resp)
    {
        printf("Got sample!\n");
        hexdump(resp->getPayload(), resp->getPayloadLength());
    }

    bool onResponse(Command *cmd, Response *resp, int retries)
    {
        cmd->print();
        resp->print();
        bool parsable = parser.parse(resp);
        return true;
    }

    int run()
    {
        protocol.start();
        protocol.setSamplePacketHandler(Protocol::bindSamplePacketHandler(&Main::onSample, this));
        protocol.setResponsePacketHandler(Protocol::bindResponsePacketHandler(&Main::onResponse, this));
        protocol.init();
        protocol.sendCommand(cmdFactory.startSampling());

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
        program.protocol.getDevice().print();
        program.stop();
        exit(0);
    }
}