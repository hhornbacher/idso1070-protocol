#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "hexdump.h"
#include "Connector.h"
#include "enums.h"
#include "IDSO1070A.h"
#include "EEROMData.h"
#include "ResponsePacket.h"
#include "Command.h"
#include "CommandGenerator.h"

class Protocol
{
private:
  Connector connection;

  CommandGenerator cmdGen;

  size_t expectedResponseCount = 0;
  bool receiving = true;

  CommandQueue commandQueue;
  bool transmitting = true;

  enum States
  {
    STATE_IDLE,
    STATE_INIT,
    STATE_RAM_CHANNEL_SELECTION,
    STATE_READ_EEROM,
    STATE_SAMPLE_RATE,
    STATE_FREQ_DIV_HIGH,
    STATE_FREQ_DIV_LOW,
    STATE_GET_TIMEBASE,
    STATE_GET_TRIGGER_SOURCE,
    STATE_DONE
  } state = STATE_IDLE;

  IDSO1070A device;
  EEROMData eeromData;

  void sendCommands(CommandQueue cmd);
  void sendCommands(Command *cmd);
  void sendSettings();

  void parsePacket(ResponsePacket *packet);
  void parseAAResponse(ResponsePacket *packet);
  void parseEEResponse(ResponsePacket *packet);
  void parseFPGAResponse(ResponsePacket *packet);
  void parseStateResponse(ResponsePacket *packet);
  void parseSampleData(ResponsePacket *packet);
  void parseFreqDivLowBytes(ResponsePacket *packet);
  void parseFreqDivHighBytes(ResponsePacket *packet);
  void parseRamChannelSelection(ResponsePacket *packet);

  void syncTimeBaseFromFreqDiv();

public:
  Protocol(char *host, int port);
  ~Protocol();

  void start();
  void stop();
  void process();
  void receive();
  void transmit();
};

#endif // _PROTOCOL_H_