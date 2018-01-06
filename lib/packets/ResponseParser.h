#ifndef _RESPONSE_PARSER_H_
#define _RESPONSE_PARSER_H_

#include <cmath>

#include "../util/mapValue.h"
#include "../device/IDSO1070A.h"

#include "Response.h"

class ResponseParser
{
private:
  IDSO1070A &device;

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
  bool parseStartCapture(Response *packet);
  bool parseEEROMPage00(Response *packet);
  bool parseCoupling(Response *packet);

public:
  ResponseParser(IDSO1070A &device);
  bool parse(Response *packet);
};

#endif // _RESPONSE_PARSER_H_