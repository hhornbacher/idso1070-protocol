#ifndef _SAMPLE_PARSER_H_
#define _SAMPLE_PARSER_H_

#include <cmath>

#include "../device/IDSO1070A.h"

#include "Response.h"

class SampleParser
{
  private:
    int littlePacketStatus = 0;

  public:
    SampleParser(IDSO1070A &device);
    bool parse(Response *packet);
};

#endif // _SAMPLE_PARSER_H_