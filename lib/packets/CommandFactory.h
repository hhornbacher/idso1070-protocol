#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include <cstdio>

#include <functional>
#include <deque>

#include "../util/mapValue.h"
#include "../device/IDSO1070A.h"
#include "Command.h"

typedef std::function<Command *()> CommandGenerator;
typedef std::deque<CommandGenerator> CommandGeneratorVector;

class CommandFactory
{
public:
private:
  IDSO1070A &device;
  CommandGeneratorVector buffer;
  Command::ResponseHandler handler;

public:
  CommandFactory(IDSO1070A &device);

  void setHandler(Command::ResponseHandler handler);

  CommandGeneratorVector &init(bool internal = false);
  CommandGeneratorVector &channelStatusOnly(bool internal = false);
  CommandGeneratorVector &channelStatus(bool internal = false);
  CommandGeneratorVector &channel1VoltageDiv(bool internal = false);
  CommandGeneratorVector &channel2VoltageDiv(bool internal = false);
  CommandGeneratorVector &levels(bool internal = false);
  CommandGeneratorVector &pullSamples(bool internal = false);
  CommandGeneratorVector &readEEROMandFPGA(bool internal = false);
  CommandGeneratorVector &updateTimeBase(bool internal = false);
  CommandGeneratorVector &trigger(bool internal = false);
  CommandGeneratorVector &triggerSource(bool internal = false);
  CommandGeneratorVector &voltageDiv(bool internal = false);
  CommandGeneratorVector &updateXTriggerPos(bool internal = false);

  CommandGenerator selectChannel();
  CommandGenerator selectRAMChannel();
  CommandGenerator readFPGAVersion();
  CommandGenerator readBatteryLevel();
  CommandGenerator readRamCount();
  CommandGenerator getFreqDivLowBytes();
  CommandGenerator getFreqDivHighBytes();
  CommandGenerator updateSampleRate();
  CommandGenerator updateTriggerSourceAndSlope();
  CommandGenerator updateTriggerLevel();
  CommandGenerator updateChannelVolts125();
  CommandGenerator updateTriggerMode();
  CommandGenerator relay1();
  CommandGenerator relay2();
  CommandGenerator relay3();
  CommandGenerator relay4();
  CommandGenerator channel1Level();
  CommandGenerator channel2Level();
  CommandGenerator channel1Coupling();
  CommandGenerator channel2Coupling();
  CommandGenerator preTrigger();
  CommandGenerator postTrigger();
  CommandGenerator startSampling();
  CommandGenerator readEEROMPage(uint8_t address);
  CommandGenerator updateTriggerPWM(uint16_t pwm);
  CommandGenerator channel1PWM(uint16_t pwm);
  CommandGenerator channel2PWM(uint16_t pwm);
};

#endif // _COMMANDS_GENERATOR_H_