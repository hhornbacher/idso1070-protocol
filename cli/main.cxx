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

Protocol protocol(device);

int main()
{
    try
    {
        protocol.start();

        printf("FPGA Version: %s\n", protocol.getFPGAFirmwareVersion().c_str());
        printf("ARM Version: %s\n", protocol.getARMFirmwareVersion().c_str());
        printf("Battery level: %u%%\n", protocol.getBatteryLevel());

        protocol.updateSampleRate();
        protocol.setFreqDiv(0x04);
        protocol.setChannelSelection(true, true);
        protocol.setTriggerConfig(Protocol::Channel1, Protocol::Analog, Protocol::Rising);
        protocol.setTriggerLevel(100, 0.0, (float)Protocol::MaxPWM);
        // std::vector<uint8_t> buffer;
        // protocol.readEEPROMPage(0x00, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x04, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x05, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x07, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x08, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x09, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x0a, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x0b, buffer);
        // buffer.clear();
        // sleep(1);
        // protocol.readEEPROMPage(0x0c, buffer);
        // buffer.clear();

        protocol.stop();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
