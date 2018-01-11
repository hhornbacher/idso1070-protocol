#ifndef _SAMPLE_PARSER_H_
#define _SAMPLE_PARSER_H_

#include <cmath>

#include "../device/IDSO1070A.h"

#include "Sample.h"

class SampleParser
{
private:
  IDSO1070A &device;

  void parseSamplePacket(Sample *packet, int index);
  void parseBothChannelsData(Sample *packet, int index);
  void parseChannel1Data(Sample *packet, int index);
  void parseChannel2Data(Sample *packet, int index);

public:
  SampleParser(IDSO1070A &device);
  void parse(Sample *packet);
};

#endif // _SAMPLE_PARSER_H_