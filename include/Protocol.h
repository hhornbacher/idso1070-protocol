#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <deque>
#include <functional>

#include "util/Timeout.h"

#include "connection/TCPConnector.h"
#include "connection/USBConnector.h"

#include "device/IDSO1070.h"

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

  Protocol();
  ~Protocol();

  void connect(string serialDevice);
  void connect(string serverHost, int port);
  void disconnect();
  void process();

  IDSO1070 &getDevice();

  void sendCommand(CommandGenerator cmdFn);

  void init();
  void startSampling();

  void setProgressHandler(ProgressHandler handler);

  bool isSampling();

  string getConnectError();
  Connector *getConnector();

  template <class F, class S>
  static ProgressHandler bindProgressHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1);
  }

private:
  // Device class to store the device's states and resources
  IDSO1070 device;

  // Connector abstraction for USB/TCP communication
  Connector *connector = NULL;
  string connectError;

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
};

#endif // _PROTOCOL_H_