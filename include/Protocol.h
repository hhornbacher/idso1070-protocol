#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "base.h"
#include "IDSO1070.h"

#include "connection/TCPConnector.h"
#include "connection/USBConnector.h"

#include "packets/SampleBuffer.h"
#include "packets/Sample.h"
#include "packets/Response.h"
#include "packets/Command.h"
#include "packets/CommandFactory.h"

#include <mutex>

class Protocol
{
public:
  // Definition of event handler types
  typedef function<void(float)> ProgressHandler;
  typedef function<void(Connector::Exception &)> ConnectionLostHandler;

  struct Transmission
  {
    Command command;
    Response response;

    Transmission(Command &command, Response &response) : command(command), response(response) {}
  };
  typedef vector<Transmission *> TransmissionLog;

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
  void setCaptureMode(CaptureMode captureMode, Command::ResponseHandler responseHandler);

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

  Connector *getConnector();
  void fetchSamples(SampleBuffer &buffer);
  void fetchDevice(IDSO1070 &dev);

  void fetchTransmissionLog(TransmissionLog &log);
  void clearTransmissionLog(bool deleteObjects = true);
  void clearCommandQueue();

private:
  static const int MaxCommandRetries = 3;

  // For thread safety
  mutable mutex deviceMutex;
  // mutable mutex protocolMutex;
  mutable mutex sampleMutex;
  mutable mutex logMutex;
  mutable mutex commandMutex;

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

  // Parser and sample data buffers
  SampleBuffer sampleBuffer;
  TransmissionLog transmissionLog;

  // Internal methods (called by process())
  void receive();
  void transmit();

  // Second stage of device initialization
  void initStage2(Command::ResponseHandler finishedHandler);

  void parse(Response *packet);
  void parse(Sample *packet);

  // Response parsing
  void parseAAResponse(Response *packet);
  void parseEEResponse(Response *packet);
  void parseFPGAResponse(Response *packet);
  void parseStateResponse(Response *packet);
  void parseFreqDivLowBytes(Response *packet);
  void parseFreqDivHighBytes(Response *packet);
  void parseRamChannelSelection(Response *packet);
  void parseCh1ZeroLevel(Response *packet);
  void parseCh2ZeroLevel(Response *packet);
  void parseRelay(Response *packet);
  void parseVoltsDiv125(Response *packet);
  void parseTriggerLevel(Response *packet);
  void parseTriggerSourceAndSlope(Response *packet);
  void parseTriggerMode(Response *packet);
  void parseEEPROMPage00(Response *packet);
  void parseCoupling(Response *packet);

  // Sample parsing
  void parseSamplePacket(Sample *packet, int index);
  void parseBothChannelsData(Sample *packet, int index);
  void parseChannel1Data(Sample *packet, int index);
  void parseChannel2Data(Sample *packet, int index);
  void interpolateSamples();
};

#endif // _PROTOCOL_H_