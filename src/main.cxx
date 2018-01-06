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
    CommandFactory cmdGen;
    ResponseParser parser;

  public:
    Protocol protocol;
    Main() : wifiConnection((char *)serverIP, serverPort),
             usbConnection((char *)device),
             protocol(CONNECTION), cmdGen(protocol.getDevice()),
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
        cmdGen.setHandler(Command::bindHandler(&Main::onResponse, this));
        protocol.setSampleDataHandler(Protocol::bindSampleDataHandler(&Main::onSample, this));

        protocol.sendCommand(cmdGen.selectRAMChannel());
        protocol.sendCommand(cmdGen.readARMVersion());
        protocol.sendCommand(cmdGen.readFPGAVersion());
        protocol.sendCommands(cmdGen.readEEROMPages());
        protocol.sendCommand(cmdGen.updateSampleRate());
        protocol.sendCommand(cmdGen.getFreqDivLowBytes());
        protocol.sendCommand(cmdGen.getFreqDivHighBytes());
        protocol.sendCommand(cmdGen.selectChannel());
        protocol.sendCommand(cmdGen.updateTriggerSourceAndSlope());
        protocol.sendCommand(cmdGen.updateTriggerLevel());
        protocol.sendCommand(cmdGen.preTrigger());
        protocol.sendCommand(cmdGen.postTrigger());
        protocol.sendCommand(cmdGen.readRamCount());
        protocol.sendCommand(cmdGen.selectRAMChannel());
        protocol.sendCommand(cmdGen.updateChannelVolts125());
        protocol.sendCommand(cmdGen.relay1());
        protocol.sendCommand(cmdGen.relay2());
        protocol.sendCommand(cmdGen.relay3());
        protocol.sendCommand(cmdGen.relay4());
        protocol.sendCommand(cmdGen.channel1Level());
        protocol.sendCommand(cmdGen.channel2Level());
        protocol.sendCommand(cmdGen.updateChannelVolts125());
        protocol.sendCommand(cmdGen.updateTriggerMode());
        protocol.sendCommand(cmdGen.updateTriggerLevel());
        protocol.sendCommand(cmdGen.channel1Coupling());
        protocol.sendCommand(cmdGen.channel2Coupling());
        protocol.sendCommand(cmdGen.startSampling());

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