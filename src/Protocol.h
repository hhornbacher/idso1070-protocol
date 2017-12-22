#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <pthread.h>

#include "Connector.h"
#include "ResponsePacket.h"
#include "Command.h"
#include "CommandsGenerator.h"

enum ReplyTypes
{
  START_CAPTURE_REPLY = 2,
  RELAY_REPLY = 3,
  TRIGGER_SOURCE_AND_SLOPE_REPLY = 5,
  VOLTS_DIV_125_REPLY = 6,
  CH1_ZERO_LEVEL_REPLY = 11,
  CH2_ZERO_LEVEL_REPLY = 12,
  TRIGGER_LEVEL_REPLY = 13,
  FREQ_DIV_LOW_2BYTES_REPLY = 18,
  FREQ_DIV_HIGH_2BYTES_REPLY = 19,
  RAM_CHANNEL_SELECT_REPLY = 21
};

class Protocol
{
private:
  Connector connection;

  CommandsGenerator cmdGen;

  PacketQueue packetQueue;
  pthread_t receiveThread;
  bool receiving = true;

  CommandQueue commandQueue;
  pthread_t transmitThread;
  bool transmitting = true;

  void sendCommands(CommandQueue cmd);
  void sendCommand(Command *cmd);
  void waitForPackets(size_t count);

public:
  Protocol(char *host, int port);
  ~Protocol();

  void start();
  void stop();
  void process();

  static void *receive(void *arg);
  static void *transmit(void *arg);
};

#endif // _PROTOCOL_H_