#ifndef _IDSO1070A_H_
#define _IDSO1070A_H_

#include <cstdio>
#include <cstdint>

#include "../util/hexdump.h"
#include "../enums.h"

#include "Channel.h"
#include "Trigger.h"

#define IDSO1070A_MAX_SAMPLE 248
#define IDSO1070A_MAX_PWM 4095
#define IDSO1070A_MEMORY_DEPTH 2000
#define IDSO1070A_SAMPLES_COUNT_PER_PACKET 500

class IDSO1070A
{
private:
  TimeBase timeBase = HDIV_1mS;
  CaptureMode captureMode = CAPMODE_NORMAL;
  ScopeMode scopeMode = SCOMODE_ANALOG;

  Channel channel1;
  Channel channel2;
  Channel *selectedChannel = &channel1;
  Trigger trigger;

  uint8_t batteryLevel = 0;
  uint8_t receiveFreqDivStatus = 0;
  uint32_t freqDiv = 0;

public:
  TimeBase getTimeBase();
  void setTimeBase();

  CaptureMode getCaptureMode();
  void setCaptureMode(CaptureMode captureMode);

  ScopeMode getScopeMode();
  void setScopeMode(ScopeMode scopeMode);

  Channel &getChannel1();
  Channel &getChannel2();

  Channel &getSelectedChannel();
  void setSelectedChannel(Channel &channel);

  Trigger &getTrigger();

  void setBatteryLevel(uint8_t batteryLevel);
  uint8_t getBatteryLevel();

  void setReceiveFreqDivStatus(uint8_t receiveFreqDivStatus);
  uint8_t getReceiveFreqDivStatus();

  void setFreqDiv(uint32_t freqDiv);
  uint32_t getFreqDiv();

  bool isSampleRate200Mor250M();

  TimeBase getTimebaseFromFreqDiv();

  size_t getSamplesNumberOfOneFrame();
  uint8_t getEnabledChannelsCount();
  uint8_t getPacketsNumber();

  void print();
};

#endif // _IDSO1070A_H_