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
#include "packets/Sample.h"
#include "packets/Response.h"
#include "packets/PacketParser.h"
#include "packets/Command.h"
#include "packets/CommandFactory.h"

class Protocol
{
public:
  static const int MaxCommandRetries = 3;

  typedef std::function<void(float)> ProgressHandler;

private:
  // Device class to store the device's states and resources
  IDSO1070A device;

  // Connector abstraction for USB/TCP communication
  Connector &connection;

  ProgressHandler progressHandler;

  // Command dispatching related members
  CommandFactory cmdFactory;
  std::deque<CommandGenerator> commandQueue;
  int commandCount = 0;
  int retries = 0;
  Timeout commandTimeout;
  Command *currentCommand = NULL;

  // Response handling related members
  Response *currentResponse = NULL;
  bool ignoreNextResponse = false;

  // Sample parsing related members
  bool sampling = false;

  PacketParser packetParser;

  void receive();
  void transmit();

public:
  Protocol(Connector &connection);
  ~Protocol();

  template <class F, class S>
  static ProgressHandler bindProgressHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1);
  }

  // Setup the connection for operation
  void start();

  // Stop and release resources
  void stop();

  // This has to be called in the main loop
  void process();

  IDSO1070A &getDevice();

  // Send one command
  void sendCommand(CommandGenerator cmdFn);

  // Send several commands
  void sendCommands(CommandGeneratorVector cmdFns);

  // Send all commands required for initialization of the device
  void init();

  void setProgressHandler(ProgressHandler handler);

  void print();
};

#endif // _PROTOCOL_H_