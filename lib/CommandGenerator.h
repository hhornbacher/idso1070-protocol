#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include <cstdio>

#include "IDSO1070A.h"
#include "Command.h"

class CommandGenerator
{
private:
  IDSO1070A *device;

public:
  CommandGenerator(IDSO1070A *device);
  float mapValue(int i, float f, float f2, float f3, float f4);
  float mapValue(float f, float f2, float f3, float f4, float f5);

  Command *readEEROMPage(uint8_t address);
  Command *selectChannel();
  Command *selectRAMChannel();
  Command *readFPGAVersion();
  Command *readBatteryLevel();
  Command *readRamCount();
  Command *getFreqDivLowBytes();
  Command *getFreqDivHighBytes();
  Command *updateSampleRate();
  Command *updateTriggerSourceAndSlope();
  Command *updateTriggerLevel();
  Command *updateChannelVolts125();
  Command *updateTriggerMode();
  Command *updateTriggerPWM(uint16_t pwm);
  Command *relay1();
  Command *relay2();
  Command *relay3();
  Command *relay4();
  Command *channel1Level();
  Command *channel2Level();
  Command *channel1Coupling();
  Command *channel2Coupling();
  Command *preTrigger();
  Command *postTrigger();
  Command *startSampling();
  Command *channel1PWM(uint16_t pwm);
  Command *channel2PWM(uint16_t pwm);
};

#endif // _COMMANDS_GENERATOR_H_