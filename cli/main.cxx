#include <util.h>

#include <Protocol.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <termio.h>

const char device[] = "/dev/ttyACM0";

Protocol protocol(
    device);

int main()
{
    try
    {
        protocol.start();

        Request request(
            Request::Control,
            Request::ReadFPGAFirmwareVersion);

        protocol.send(request,
                      [](Response &response) { printf("Response handler:\n%s\n", response.toString().c_str()); },
                      [](std::string msg) { printf("Error handler:\n%s\n", msg.c_str()); });
        protocol.send(request,
                      [](Response &response) { printf("Response handler:\n%s\n", response.toString().c_str()); },
                      [](std::string msg) { printf("Error handler:\n%s\n", msg.c_str()); });

        // printf("FPGA Version: %s\n", protocol.getFPGAFirmwareVersion().c_str());
        // printf("ARM Version: %s\n", protocol.getARMFirmwareVersion().c_str());
        // printf("Battery level: %u%%\n", protocol.getBatteryLevel());

        // protocol.updateSampleRate();
        // protocol.setFreqDiv(0x04);
        // protocol.setChannelSelection(true, true);
        // protocol.setTriggerConfig(Protocol::Channel1, Protocol::Analog, Protocol::Rising);
        // protocol.setTriggerLevel(100, 0.0, (float)Protocol::MaxPWM);
        // protocol.setPreTriggerLength(250, 0.5);
        // protocol.setPostTriggerLength(250, 0.5);
        // protocol.getRAMCount(2, 250, 0.5, 1);
        // protocol.startSampling();

        protocol.stop();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
