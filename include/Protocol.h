#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "base.h"
#include "util/Timeout.h"

#include "connection/TCPConnector.h"
#include "connection/USBConnector.h"

#include "device/IDSO1070.h"

#include "packets/Sample.h"
#include "packets/Response.h"
#include "packets/PacketParser.h"
#include "packets/Command.h"
#include "packets/CommandFactory.h"

class Protocol
{
public:
  static const int MaxCommandRetries = 3;

  typedef function<void(float)> ProgressHandler;
  typedef Command::ResponseHandler BatchFinishedHandler;

  Protocol();
  ~Protocol();

  void connect(string serialDevice);
  void connect(string serverHost, int port);
  void disconnect();
  void process();

  IDSO1070 &getDevice();

  void sendCommand(Command *cmd);
  void sendCommandBatch(deque<Command *> cmds, ProgressHandler progressHandler, BatchFinishedHandler finishedHandler);

  void init(ProgressHandler progressHandler, BatchFinishedHandler finishedHandler);
  void startSampling(Command::ResponseHandler responseHandler);

  bool isSampling();

  string getConnectError();
  Connector *getConnector();

private:
  // Device class to store the device's states and resources
  IDSO1070 device;

  // Connector abstraction for USB/TCP communication
  Connector *connector = NULL;
  string connectError;

  // Command dispatching related members
  CommandFactory cmdFactory;
  deque<Command *> commandQueue;
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

  void initStage2(ProgressHandler progressHandler, BatchFinishedHandler finishedHandler);
};

#endif // _PROTOCOL_H_