#include "SampleParser.h"

SampleParser::SampleParser(IDSO1070A &device) : device(device)
{
}

bool SampleParser::parse(Response *packet)
{

    // byte[] bytes = replyPacket.getBytes();
    // if (Utility.isSet(bytes[7], 5)) {
    //     int i = bytes[7] & 15;
    //     if (this.littlePacketStatus == i) {
    //         this.littlePacketStatus++;
    //         receiveSamplesPacket(replyPacket, i);
    //         if (i == getLastPacketIndex()) {
    //             this.littlePacketStatus = 0;
    //             fixAdDiff();
    //             interpolateSamples();
    //             boolean z = Utility.isSet(bytes[7], 6);
    //             if (this.connector.isSendingCommands()) {
    //                 C0002a.m16a("正在发送命令，波形无效！", new Object[0]);
    //                 return;
    //             }
    //             if (Utility.isSet(bytes[7], 4)) {
    //                 waveisfound = true;
    //             } else {
    //                 waveisfound = false;
    //             }
    //             notifyNewWaveFrameFound();
    //             notifyTriggerCompared(z);
    //             return;
    //         }
    //         return;
    //     }
    //     this.littlePacketStatus = 0;
    //     return;
    // }
    // C0002a.m16a("没有采集结束，返回。", new Object[0]);
}