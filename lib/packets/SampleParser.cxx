#include "SampleParser.h"

SampleParser::SampleParser(IDSO1070A &device) : device(device)
{
}

void SampleParser::parseSamplePacket(Sample *packet, int i)
{
    if (device.getEnabledChannelsCount() == 2)
    {
        parseBothChannelsData(packet, i);
    }
    else if (device.getChannel1().isEnabled() && !device.getChannel2().isEnabled())
    {
        parseChannel1Data(packet, i);
    }
    else if (device.getChannel2().isEnabled() && !device.getChannel1().isEnabled())
    {
        parseChannel2Data(packet, i);
    }
}

void SampleParser::parse(Sample *packet)
{
    uint8_t head = packet->getPayload()[0];
    if (head & (1 << 5))
    {
        int i = head & 0x0f;
        printf("head=%02x, i=%x\n", head, i);
        if (device.getLittlePacketStatus() == i)
        {
            device.setLittlePacketStatus(device.getLittlePacketStatus() + 1);
            parseSamplePacket(packet, i);
            if (i == (device.getPacketsNumber() - 1))
            {
                device.setLittlePacketStatus(0);

                //             fixAdDiff();
                //             interpolateSamples();

                //             if (this.connector.isSendingCommands()) {
                //                 return;
                //             }

                if (head & (1 << 6))
                {
                    // trigger compared
                }

                if (head & (1 << 4))
                {
                    // wave found
                }
                else
                {
                    // wave not found
                }

                return;
            }
            return;
        }
        device.setLittlePacketStatus(0);
        return;
    }
}