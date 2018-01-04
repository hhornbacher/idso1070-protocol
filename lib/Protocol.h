#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unistd.h>

#include "util/hexdump.h"
#include "util/Timeout.h"

#include "connection/TCPConnector.h"
#include "connection/USBConnector.h"

#include "device/IDSO1070A.h"
#include "device/EEROMData.h"

#include "enums.h"
#include "packets/Response.h"
#include "packets/Command.h"
#include "packets/CommandQueue.h"
#include "packets/CommandGenerator.h"

// typedef std::function<void(Commands cmd, bool success)> ResponseHandler;

// class ResponseHandler
// {
// public:
//   virtual bool onResponse(Commands cmd, bool success) = 0;
// };

class Protocol
{
private:
  Connector *connection;

  CommandGenerator cmdGen;

  size_t expectedResponseCount = 0;
  bool requestSuccess = false;
  // ResponseHandler *responseHandler;
  Commands lastCommand;
  CommandQueue commandQueue;
  Timeout commandTimeout;
  bool sampling = false;

  Timeout readBatteryTimeout;

  enum States
  {
    STATE_IDLE,
    STATE_REQUEST,
    STATE_RESPONSE,
    STATE_DONE
  } state = STATE_IDLE;

  IDSO1070A device;
  EEROMData eeromData;

  void resolveCommandResponse(Response *packet);
  void rejectCommandResponse(Response *packet);

  void parsePacket(Response *packet);
  void parseAAResponse(Response *packet);
  void parseEEResponse(Response *packet);
  void parseFPGAResponse(Response *packet);
  void parseStateResponse(Response *packet);
  void parseSampleData(Response *packet);
  void parseFreqDivLowBytes(Response *packet);
  void parseFreqDivHighBytes(Response *packet);
  void parseRamChannelSelection(Response *packet);
  void parseCh1ZeroLevel(Response *packet);
  void parseCh2ZeroLevel(Response *packet);
  void parseRelay(Response *packet);
  void parseVoltsDiv125(Response *packet);
  void parseTriggerLevel(Response *packet);
  void parseTriggerSourceAndSlope(Response *packet);
  void parseStartCapture(Response *packet);
  void parseEEROMPage00(Response *packet);
  void parseCoupling(Response *packet);

  void syncTimeBaseFromFreqDiv();

  void changeState(States state);

  Command *getCommand(Commands cmd);

public:
  Protocol(Connector *connection);
  ~Protocol();

  void start();
  void stop();
  void process();
  IDSO1070A &getDevice();
  EEROMData &getEEROMData();

  void sendCommand(Commands cmd);

  void resendLastCommand();

  void receive();
  void transmit();

  void print();
};

#endif // _PROTOCOL_H_