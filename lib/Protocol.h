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

#define COMMAND_MAX_RETRIES 3

class Protocol
{
public:
  typedef std::function<void(Response &)> SampleDataHandler;

private:
  IDSO1070A device;

  Connector &connection;

  bool sampling = false;
  SampleDataHandler sampleDataHandler;

  std::deque<CommandGenerator> commandQueue;
  int retries = 0;
  bool ignoreNextResponse = false;
  Timeout commandTimeout;

  Command *currentCommand = NULL;
  Response *currentResponse = NULL;

public:
  Protocol(Connector &connection);
  ~Protocol();

  void start();
  void stop();
  void process();

  IDSO1070A &getDevice();

  void sendCommand(CommandGenerator cmdFn);
  void sendCommands(CommandGeneratorVector cmdFns);

  void receive();
  void transmit();

  void print();
};

#endif // _PROTOCOL_H_