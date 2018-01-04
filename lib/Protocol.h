#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unistd.h>

#include "util/hexdump.h"
#include "Timeout.h"
#include "connection/TCPConnector.h"
#include "connection/USBConnector.h"
#include "enums.h"
#include "device/IDSO1070A.h"
#include "device/EEROMData.h"
#include "ResponsePacket.h"
#include "Command.h"
#include "CommandGenerator.h"

class ResponseHandler
{
public:
  virtual bool onResponse(Commands cmd, bool success) = 0;
};

class Protocol
{
private:
  Connector *connection;

  CommandGenerator cmdGen;

  size_t expectedResponseCount = 0;
  bool requestSuccess = false;
  ResponseHandler *responseHandler;
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

  void resolveCommandResponse(ResponsePacket *packet);
  void rejectCommandResponse(ResponsePacket *packet);

  void parsePacket(ResponsePacket *packet);
  void parseAAResponse(ResponsePacket *packet);
  void parseEEResponse(ResponsePacket *packet);
  void parseFPGAResponse(ResponsePacket *packet);
  void parseStateResponse(ResponsePacket *packet);
  void parseSampleData(ResponsePacket *packet);
  void parseFreqDivLowBytes(ResponsePacket *packet);
  void parseFreqDivHighBytes(ResponsePacket *packet);
  void parseRamChannelSelection(ResponsePacket *packet);
  void parseCh1ZeroLevel(ResponsePacket *packet);
  void parseCh2ZeroLevel(ResponsePacket *packet);
  void parseRelay(ResponsePacket *packet);
  void parseVoltsDiv125(ResponsePacket *packet);
  void parseTriggerLevel(ResponsePacket *packet);
  void parseTriggerSourceAndSlope(ResponsePacket *packet);
  void parseStartCapture(ResponsePacket *packet);
  void parseEEROMPage00(ResponsePacket *packet);
  void parseCoupling(ResponsePacket *packet);

  void syncTimeBaseFromFreqDiv();

  void receive();
  void transmit();

  void changeState(States state);

  Command *getCommand(Commands cmd);

public:
  Protocol(Connector *connection, ResponseHandler *responseHandler);
  ~Protocol();

  void start();
  void stop();
  void process();
  IDSO1070A &getDevice();
  EEROMData &getEEROMData();

  void sendCommand(Commands cmd);

  void resendLastCommand();

  void print();
};

#endif // _PROTOCOL_H_