#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include "Protocol.h"
#include "util/FIFOSerializer.h"

void sigHandler(int sig);

#define CONNECTION wifiConnection

class Main
{
  private:
    const char *outChannel1Path = "./channel1-int16-raw.fifo";
    // const char *outChannel2Path = "./channel2-int16-raw.fifo";

    bool runProgram = true;
    const char *device = "/dev/ttyACM0";

    int resendCounter = 0;
    const char *serverIP = "192.168.1.1";
    const uint16_t serverPort = 8870;

    TCPConnector wifiConnection;
    USBConnector usbConnection;
    CommandFactory cmdFactory;

    FIFOSerializer outChannel1;
    // FIFOSerializer outChannel2;

  public:
    Protocol protocol;
    Main() : wifiConnection((char *)serverIP, serverPort),
             usbConnection((char *)device),
             protocol(CONNECTION), cmdFactory(protocol.getDevice()),
             outChannel1(outChannel1Path, true) //, outChannel2(outChannel2Path, true)
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
        protocol.start();
        protocol.setProgressHandler(Protocol::bindProgressHandler(&Main::onProgress, this));
        protocol.init();
        protocol.sendCommand(cmdFactory.startSampling());

        while (runProgram)
        {
            protocol.process();

            while (protocol.getDevice().getChannel1().getSampleBuffer().size() > 0)
            {
                Channel::SampleBuffer &buffer = protocol.getDevice().getChannel1().getSampleBuffer();
                int16_t data = *buffer.begin();
                outChannel1.serialize(&data, 1);
                buffer.pop_front();
            }
            // while (protocol.getDevice().getChannel2().getSampleBuffer().size() > 0)
            // {
            //     Channel::SampleBuffer &buffer = protocol.getDevice().getChannel2().getSampleBuffer();
            //     int16_t data = *buffer.begin();
            //     outChannel2.serialize(&data, 1);
            //     buffer.pop_front();
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
        program.protocol.getDevice().print();
        program.stop();
        exit(0);
    }
}