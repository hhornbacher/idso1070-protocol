#ifndef _RESPONSE_PARSER_H_
#define _RESPONSE_PARSER_H_

#include "base.h"
#include "util.h"
#include "IDSO1070.h"
#include "Response.h"
#include "Sample.h"

class PacketParser
{
public:
  PacketParser(IDSO1070 &device, Sample::SampleBuffer &sampleBuffer1, Sample::SampleBuffer &sampleBuffer2);

  void parse(Response *packet);
  void parse(Sample *packet);

private:
  IDSO1070 &device;
  Sample::SampleBuffer &sampleBuffer1;
  Sample::SampleBuffer &sampleBuffer2;

  // Response parsing
  void parseAAResponse(Response *packet);
  void parseEEResponse(Response *packet);
  void parseFPGAResponse(Response *packet);
  void parseStateResponse(Response *packet);
  void parseFreqDivLowBytes(Response *packet);
  void parseFreqDivHighBytes(Response *packet);
  void parseRamChannelSelection(Response *packet);
  void parseCh1ZeroLevel(Response *packet);
  void parseCh2ZeroLevel(Response *packet);
  void parseRelay(Response *packet);
  void parseVoltsDiv125(Response *packet);
  void parseTriggerLevel(Response *packet);
  void parseTriggerSourceAndSlope(Response *packet);
  void parseTriggerMode(Response *packet);
  void parseEEPROMPage00(Response *packet);
  void parseCoupling(Response *packet);

  // Sample parsing
  void parseSamplePacket(Sample *packet, int index);
  void parseBothChannelsData(Sample *packet, int index);
  void parseChannel1Data(Sample *packet, int index);
  void parseChannel2Data(Sample *packet, int index);
  void fixAdDiff();
  void fixCh1AdDiff();
  void fixCh2AdDiff();
  void interpolateSamples();
};

#endif // _RESPONSE_PARSER_H_