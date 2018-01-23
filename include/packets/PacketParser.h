#ifndef _RESPONSE_PARSER_H_
#define _RESPONSE_PARSER_H_

#include "base.h"
#include "util/mapValue.h"
#include "device/IDSO1070.h"
#include "Response.h"
#include "Sample.h"

class PacketParser
{
public:
  PacketParser(IDSO1070 &device);

  bool parse(Response *packet);
  void parse(Sample *packet);

private:
  IDSO1070 &device;

  // Response parsing
  bool parseAAResponse(Response *packet);
  bool parseEEResponse(Response *packet);
  bool parseFPGAResponse(Response *packet);
  bool parseStateResponse(Response *packet);
  bool parseFreqDivLowBytes(Response *packet);
  bool parseFreqDivHighBytes(Response *packet);
  bool parseRamChannelSelection(Response *packet);
  bool parseCh1ZeroLevel(Response *packet);
  bool parseCh2ZeroLevel(Response *packet);
  bool parseRelay(Response *packet);
  bool parseVoltsDiv125(Response *packet);
  bool parseTriggerLevel(Response *packet);
  bool parseTriggerSourceAndSlope(Response *packet);
  bool parseTriggerMode(Response *packet);
  bool parseEEROMPage00(Response *packet);
  bool parseCoupling(Response *packet);

  // Sample parsing
  void parseSamplePacket(Sample *packet, int index);
  void parseBothChannelsData(Sample *packet, int index);
  void parseChannel1Data(Sample *packet, int index);
  void parseChannel2Data(Sample *packet, int index);
  void fixAdDiff();
  // void fixCh1AdDiff();
  // void fixCh2AdDiff();
  void interpolateSamples();
};

#endif // _RESPONSE_PARSER_H_