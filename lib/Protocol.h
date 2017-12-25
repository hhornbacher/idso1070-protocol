#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <pthread.h>

#include "hexdump.h"
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

  struct IDSO1070A
  {
    bool channel1;
    bool channel2;

    void print()
    {
      printf(
          "[IDSO1070A]\n"
          "channel1 = %d\n"
          "channel2 = %d\n"
          "\n\n",
          channel1, channel2);
    }
  } device = {
      .channel1 = false,
      .channel2 = false};

  struct EEROMData
  {
    uint8_t diffFixData[2][256];
    uint8_t caliLevel[200];
    uint8_t fpgaAlert[40];
    uint8_t productName[20];
    uint8_t userName[12];

    void print()
    {
      printf(
          "[EEROMData]\n"
          "diffFixData[0]:\n");
      hexdump(diffFixData[0], 256);
      printf("diffFixData[1]:\n");
      hexdump(diffFixData[1], 256);
      printf("caliLevel:\n");
      hexdump(caliLevel, 200);
      printf("fpgaAlert:\n");
      hexdump(fpgaAlert, 40);
      printf(
          "productName = %s\n"
          "userName = %s\n"
          "\n\n",
          productName, userName);
    }
  } eeromData;

  void sendCommands(CommandQueue cmd);
  void sendCommand(Command *cmd);
  void waitForPackets(size_t count);

  void parsePacket(ResponsePacket *packet);
  void parseAAResponse(ResponsePacket *packet);
  void parseEEResponse(ResponsePacket *packet);
  void parseFPGAResponse(ResponsePacket *packet);
  void parseSampleData(ResponsePacket *packet);
  void parseEEROMPage00(ResponsePacket *packet);
  void parseEEROMPage04(ResponsePacket *packet);
  void parseEEROMPage05(ResponsePacket *packet);
  void parseEEROMPage07(ResponsePacket *packet);
  void parseEEROMPage08(ResponsePacket *packet);
  void parseEEROMPage09(ResponsePacket *packet);
  void parseEEROMPage0a(ResponsePacket *packet);
  void parseEEROMPage0b(ResponsePacket *packet);
  void parseEEROMPage0c(ResponsePacket *packet);
  void parseStartCapture(ResponsePacket *packet);
  void parseRelay(ResponsePacket *packet);
  void parseTriggerSourceAndSlope(ResponsePacket *packet);
  void parseVoltsDiv125(ResponsePacket *packet);
  void parseCh1ZeroLevel(ResponsePacket *packet);
  void parseCh2ZeroLevel(ResponsePacket *packet);
  void parseTriggerLevel(ResponsePacket *packet);
  void parseFreqDivLowBytes(ResponsePacket *packet);
  void parseFreqDivHighBytes(ResponsePacket *packet);
  void parseRamChannelSelection(ResponsePacket *packet);

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