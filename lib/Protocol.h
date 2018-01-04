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

#include "enums.h"
#include "packets/Response.h"
#include "packets/ResponseParser.h"
#include "packets/Command.h"
#include "packets/CommandQueue.h"
#include "packets/CommandGenerator.h"

#define COMMAND_MAX_RETRIES 3

class Protocol
{
private:
  enum States
  {
    STATE_IDLE,
    STATE_REQUEST,
    STATE_RESPONSE,
    STATE_DONE
  } state = STATE_IDLE;

  IDSO1070A device;

  Connector *connection;

  CommandGenerator cmdGen;

  ResponseParser parser;

  bool sampling = false;

  int retries = 0;
  size_t expectedResponseCount = 0;
  CommandQueue commandQueue;
  Timeout commandTimeout;

  Response *lastResponse;

  // Timeout readBatteryTimeout;

  void resolveCommandResponse(Response *packet);
  void rejectCommandResponse(Response *packet);

  void changeState(States state);

  Command *getCommand(Commands cmd);

public:
  Protocol(Connector *connection);
  ~Protocol();

  void start();
  void stop();
  void process();

  IDSO1070A &getDevice();

  // void sendCommand(Commands cmd);
  void sendCommand(Command *cmd);

  void receive();
  void transmit();

  void print();
};

#endif // _PROTOCOL_H_