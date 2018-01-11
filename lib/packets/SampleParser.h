#ifndef _SAMPLE_PARSER_H_
#define _SAMPLE_PARSER_H_

#include <cmath>

#include "../device/IDSO1070A.h"

#include "Sample.h"

class SampleParser
{
private:
  IDSO1070A &device;

  void parseSamplePacket(Sample *packet, int i);
  void parseBothChannelsData(Sample *packet, int i);
  void parseChannel1Data(Sample *packet, int i);
  void parseChannel2Data(Sample *packet, int i);

public:
  SampleParser(IDSO1070A &device);
  void parse(Sample *packet);
};

#endif // _SAMPLE_PARSER_H_