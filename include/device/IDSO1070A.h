#ifndef _IDSO1070A_H_
#define _IDSO1070A_H_

#include <cstdio>
#include <cstdint>

#include "../util/hexdump.h"
#include "../enums.h"

#include "EEROMData.h"
#include "Channel.h"
#include "Trigger.h"

class IDSO1070A
{
public:
  static const int MaxPWM = 4095;
  static const int MemoryDepth = 2000;
  static const int SamplesCountPerPacket = 500;

private:
  TimeBase timeBase = HDIV_1mS;
  CaptureMode captureMode = CAPMODE_NORMAL;
  ScopeMode scopeMode = SCOMODE_ANALOG;

  EEROMData eeromData;
  Channel channel1;
  Channel channel2;
  Channel *selectedChannel = &channel1;
  Trigger trigger;

  uint8_t batteryLevel = 0;

  int littlePacketStatus = 0;

  uint8_t receiveFreqDivStatus = 0;
  uint32_t freqDiv = 0;

  char armFirmwareVersion[9];
  char fpgaFirmwareVersion[9];

public:
  IDSO1070A();
  TimeBase getTimeBase();
  void setTimeBase(TimeBase timeBase);

  CaptureMode getCaptureMode();
  void setCaptureMode(CaptureMode captureMode);

  ScopeMode getScopeMode();
  void setScopeMode(ScopeMode scopeMode);

  EEROMData &getEEROMData();
  Channel &getChannel1();
  Channel &getChannel2();

  Channel &getSelectedChannel();
  void setSelectedChannel(Channel &channel);

  Trigger &getTrigger();

  void setBatteryLevel(uint8_t batteryLevel);
  uint8_t getBatteryLevel();

  void setLittlePacketStatus(int littlePacketStatus);
  int getLittlePacketStatus();

  void setReceiveFreqDivStatus(uint8_t receiveFreqDivStatus);
  uint8_t getReceiveFreqDivStatus();

  void setFreqDiv(uint32_t freqDiv);
  uint32_t getFreqDiv();

  void setARMFirmwareVersion(char *version);
  void setFPGAFirmwareVersion(char *version);

  bool isSampleRate200Mor250M();

  TimeBase getTimebaseFromFreqDiv();

  size_t getSamplesNumberOfOneFrame();
  uint8_t getEnabledChannelsCount();
  uint8_t getPacketsNumber();

  void print();
};

#endif // _IDSO1070A_H_