#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "base.h"

#include "connection/TCPConnector.h"
#include "connection/USBConnector.h"

#include "IDSO1070.h"

#include "packets/Sample.h"
#include "packets/Response.h"
#include "packets/PacketParser.h"
#include "packets/Command.h"
#include "packets/CommandFactory.h"

class Protocol
{
public:
  // Definition of event handler types
  typedef function<void(float)> ProgressHandler;
  typedef function<void(Connector::Exception &)> ConnectionLostHandler;

  Protocol();
  ~Protocol();

  // Connection control
  void connect(string serialDevice);
  void connect(string serverHost, int port);
  void disconnect();

  // Called in main loop
  void process();

  void sendCommand(Command *cmd);
  void sendCommandBatch(deque<Command *> cmds, Command::ResponseHandler finishedHandler);

  // Device control
  void init(Command::ResponseHandler responseHandler);
  void readBatteryLevel(Command::ResponseHandler responseHandler);
  void setTimeBase(TimeBase timeBase, Command::ResponseHandler responseHandler);
  void setScopeMode(ScopeMode scopeMode, Command::ResponseHandler responseHandler);

  // Channel control
  void enableChannel(ChannelSelector channel, Command::ResponseHandler responseHandler);
  void disableChannel(ChannelSelector channel, Command::ResponseHandler responseHandler);
  void setChannelVerticalDiv(ChannelSelector channel, VoltageDiv div, Command::ResponseHandler responseHandler);
  void setChannelCoupling(ChannelSelector channel, InputCoupling coupling, Command::ResponseHandler responseHandler);
  void setChannelVerticalPosition(ChannelSelector channel, uint16_t pos, Command::ResponseHandler responseHandler);

  // Trigger control
  void setTriggerMode(TriggerMode mode, Command::ResponseHandler responseHandler);
  void setTriggerChannel(TriggerChannel channel, Command::ResponseHandler responseHandler);
  void setTriggerSlope(TriggerSlope slope, Command::ResponseHandler responseHandler);
  void setTriggerLevel(uint16_t level, Command::ResponseHandler responseHandler);

  // Sampling control
  void startSampling(Command::ResponseHandler responseHandler);
  void stopSampling(Command::ResponseHandler responseHandler);

  // Set event handlers
  void setConnectionLostHandler(ConnectionLostHandler connectionLostHandler);
  void setProgressHandler(ProgressHandler progressHandler);

  bool isSampling();
  IDSO1070 &getDevice();
  Connector *getConnector();

private:
  static const int MaxCommandRetries = 3;

  // Device class to store the device's states and resources
  IDSO1070 device;

  // Connector abstraction for USB/TCP communication
  Connector *connector = NULL;
  ConnectionLostHandler connectionLostHandler;

  ProgressHandler progressHandler;

  // Command dispatching related members
  CommandFactory cmdFactory;
  deque<Command *> commandQueue;
  int retries = 0;
  Command *currentCommand = NULL;

  // Response handling related members
  Response *currentResponse = NULL;
  bool ignoreNextResponse = false;

  // Sample parsing related members
  bool sampling = false;

  // Parser and sample data buffers
  PacketParser packetParser;
  Sample::SampleBuffer sampleBuffer1;
  Sample::SampleBuffer sampleBuffer2;

  // Internal methods (called by process)
  void receive();
  void transmit();

  // Second stage of device initialization
  void initStage2(Command::ResponseHandler finishedHandler);
};

#endif // _PROTOCOL_H_