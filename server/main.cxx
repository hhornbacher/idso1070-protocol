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

  public:
    Protocol protocol;
    Main() : wifiConnection((char *)serverIP, serverPort),
             usbConnection((char *)device),
             protocol(CONNECTION), cmdFactory(protocol.getDevice())
    {
        signal(SIGINT, sigHandler);
    }

    void stop()
    {
        runProgram = false;
    }

    void onProgress(float progress)
    {
        printf("\nProgress: %.2f\n", (progress * 100.0f));
    }

    int run()
    {
        Channel::SampleBuffer &buffer1 = protocol.getDevice().getChannel1().getSampleBuffer();
        Channel::SampleBuffer &buffer2 = protocol.getDevice().getChannel2().getSampleBuffer();
        protocol.start();
        protocol.setProgressHandler(Protocol::bindProgressHandler(&Main::onProgress, this));
        protocol.init();
        protocol.sendCommand(cmdFactory.startSampling());

        while (runProgram)
        {
            protocol.process();

            // if (buffer1.size() > 0)
            // {
            //     buffer1.linearize();
            //     outChannel1.serialize(buffer1.array_one().first, buffer1.size() * sizeof(int16_t));
            //     buffer1.clear();
            // }
            // if (buffer2.size() > 0)
            // {
            //     buffer2.linearize();
            //     outChannel1.serialize(buffer2.array_one(), buffer2.size());
            //     buffer2.clear();
            // }
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
        printf("\nExiting!\n\n");
        program.stop();
    }
}