#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include <cstdio>

#include "IDSO1070A.h"
#include "Command.h"

class CommandGenerator
{
private:
  Command *readEEROMPage(uint8_t address);

  float mapValue(int i, float f, float f2, float f3, float f4);
  float mapValue(float f, float f2, float f3, float f4, float f5);

public:
  CommandQueue readEEROM(IDSO1070A &device);
  CommandQueue updateTriggerSource(IDSO1070A &device);
  CommandQueue updateTimeBase(IDSO1070A &device);
  CommandQueue initialize(IDSO1070A &device);
  CommandQueue voltageDiv(IDSO1070A &device);
  CommandQueue pullSamples(IDSO1070A &device);
  CommandQueue channelStatus(IDSO1070A &device);
  CommandQueue channelStatusOnly(IDSO1070A &device);
  CommandQueue levels(IDSO1070A &device);
  CommandQueue trigger(IDSO1070A &device);
  CommandQueue xTriggerPos(IDSO1070A &device);
  CommandQueue channel1VoltageDiv(IDSO1070A &device);
  CommandQueue channel2VoltageDiv(IDSO1070A &device);

  Command *selectChannel(IDSO1070A &device);
  Command *selectRAMChannel(IDSO1070A &device);
  Command *readFPGAVersion(IDSO1070A &device);
  Command *readBatteryLevel(IDSO1070A &device);
  Command *readRamCount(IDSO1070A &device);
  Command *getFreqDivLowBytes(IDSO1070A &device);
  Command *getFreqDivHighBytes(IDSO1070A &device);
  Command *updateSampleRate(IDSO1070A &device);
  Command *updateTriggerSourceAndSlope(IDSO1070A &device);
  Command *updateTriggerLevel(IDSO1070A &device);
  Command *updateChannelVolts125(IDSO1070A &device);
  Command *updateTriggerMode(IDSO1070A &device);
  Command *updateTriggerPWM(IDSO1070A &device, uint16_t pwm);
  Command *relay1(IDSO1070A &device);
  Command *relay2(IDSO1070A &device);
  Command *relay3(IDSO1070A &device);
  Command *relay4(IDSO1070A &device);
  Command *channel1Level(IDSO1070A &device);
  Command *channel2Level(IDSO1070A &device);
  Command *channel1Coupling(IDSO1070A &device);
  Command *channel2Coupling(IDSO1070A &device);
  Command *preTrigger(IDSO1070A &device);
  Command *postTrigger(IDSO1070A &device);
  Command *channel1PWM(IDSO1070A &device, uint16_t pwm);
  Command *channel2PWM(IDSO1070A &device, uint16_t pwm);
};

#endif // _COMMANDS_GENERATOR_H_