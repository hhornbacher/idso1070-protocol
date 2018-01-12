#include "SampleParser.h"

SampleParser::SampleParser(IDSO1070A &device) : device(device)
{
}

void SampleParser::parseSamplePacket(Sample *packet, int index)
{
    if (device.getEnabledChannelsCount() == 2)
    {
        parseBothChannelsData(packet, index);
    }
    else if (device.getChannel1().isEnabled() && !device.getChannel2().isEnabled())
    {
        parseChannel1Data(packet, index);
    }
    else if (device.getChannel2().isEnabled() && !device.getChannel1().isEnabled())
    {
        parseChannel2Data(packet, index);
    }
}

void SampleParser::parseBothChannelsData(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * (IDSO1070A::SamplesCountPerPacket / 2);

    while ((pos * 2) < IDSO1070A::SamplesCountPerPacket)
    {
        if (device.getChannel1().getCoupling() == COUPLING_GND)
        {
            device.getChannel1().getSampleBuffer().push_back(device.getChannel1().getVerticalPosition());
        }
        else
        {
            device.getChannel1().getSampleBuffer().push_back((int16_t)(packet->getPayload()[1 + (pos * 2)] & 255));
        }
        if (device.getChannel2().getCoupling() == COUPLING_GND)
        {
            device.getChannel2().getSampleBuffer().push_back(device.getChannel2().getVerticalPosition());
        }
        else
        {
            device.getChannel2().getSampleBuffer().push_back((int16_t)(packet->getPayload()[1 + (pos * 2) + 1] & 255));
        }
        //     statisticCh1Max(sampleOffset + pos, this.channel1.getSamples()[sampleOffset + pos]);
        //     statisticCh1Min(sampleOffset + pos, this.channel1.getSamples()[sampleOffset + pos]);
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void SampleParser::parseChannel1Data(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * IDSO1070A::SamplesCountPerPacket;
    while (pos < IDSO1070A::SamplesCountPerPacket)
    {
        if (device.getChannel1().getCoupling() == COUPLING_GND)
        {
            device.getChannel1().getSampleBuffer().push_back(device.getChannel1().getVerticalPosition());
        }
        else
        {
            device.getChannel1().getSampleBuffer().push_back((int16_t)(packet->getPayload()[1 + pos] & 255));
        }
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void SampleParser::parseChannel2Data(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * IDSO1070A::SamplesCountPerPacket;
    while (pos < IDSO1070A::SamplesCountPerPacket)
    {
        if (device.getChannel2().getCoupling() == COUPLING_GND)
        {
            device.getChannel2().getSampleBuffer().push_back(device.getChannel2().getVerticalPosition());
        }
        else
        {
            device.getChannel2().getSampleBuffer().push_back((int16_t)(packet->getPayload()[1 + pos] & 255));
        }
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void SampleParser::parse(Sample *packet)
{
    uint8_t head = packet->getPayload()[0];
    if (head & (1 << 5))
    {
        int i = head & 0x0f;
        if (device.getLittlePacketStatus() == i)
        {
            device.setLittlePacketStatus(device.getLittlePacketStatus() + 1);
            parseSamplePacket(packet, i);
            if (i == (device.getPacketsNumber() - 1))
            {
                printf("B\n");
                device.setLittlePacketStatus(0);

                //             fixAdDiff();
                //             interpolateSamples();

                //             if (this.connector.isSendingCommands()) {
                //                 return;
                //             }

                if (head & (1 << 6))
                {
                    printf("\n\nTrigger compared\n\n");
                    // trigger compared
                }

                if (head & (1 << 4))
                {
                    printf("\n\nWave found\n\n");
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