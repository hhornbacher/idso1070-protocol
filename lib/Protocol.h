#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <deque>
#include <functional>

#include "util/hexdump.h"
#include "util/Timeout.h"

#include "connection/TCPConnector.h"
#include "connection/USBConnector.h"

#include "device/IDSO1070A.h"

#include "enums.h"
#include "packets/Response.h"
#include "packets/ResponseParser.h"
#include "packets/Command.h"
#include "packets/CommandFactory.h"

class Protocol
{
public:
  static const int MaxCommandRetries = 3;

  typedef std::function<void(Response *)> SamplePacketHandler;
  typedef std::function<bool(Command *, Response *, int)> ResponsePacketHandler;

private:
  IDSO1070A device;

  Connector &connection;

  bool sampling = false;
  SamplePacketHandler samplePacketHandler;

  std::deque<CommandGenerator> commandQueue;
  int retries = 0;
  bool ignoreNextResponse = false;
  Timeout commandTimeout;
  ResponsePacketHandler responsePacketHandler;

  Command *currentCommand = NULL;
  Response *currentResponse = NULL;

  CommandFactory cmdFactory;

public:
  Protocol(Connector &connection);
  ~Protocol();

  template <class F, class S>
  static SamplePacketHandler bindSamplePacketHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1);
  }

  template <class F, class S>
  static ResponsePacketHandler bindResponsePacketHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  }

  void start();
  void stop();
  void process();

  IDSO1070A &getDevice();

  void sendCommand(CommandGenerator cmdFn);
  void sendCommands(CommandGeneratorVector cmdFns);

  void init();

  void setSamplePacketHandler(SamplePacketHandler handler);
  void setResponsePacketHandler(ResponsePacketHandler handler);

  void receive();
  void transmit();

  void print();
};

#endif // _PROTOCOL_H_