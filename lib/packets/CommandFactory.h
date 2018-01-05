#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include <cstdio>

#include <functional>
#include <deque>

#include "../util/mapValue.h"
#include "../device/IDSO1070A.h"
#include "Command.h"

typedef std::function<Command *()> CommandGenerator;

class CommandFactory
{
private:
  IDSO1070A &device;
  std::deque<CommandGenerator> buffer;
  Command::ResponseHandler handler;

public:
  CommandFactory(IDSO1070A &device);

  void setHandler(Command::ResponseHandler handler);

  std::deque<CommandGenerator> &init(bool internal = false);
  std::deque<CommandGenerator> &channelStatusOnly(bool internal = false);
  std::deque<CommandGenerator> &channelStatus(bool internal = false);
  std::deque<CommandGenerator> &channel1VoltageDiv(bool internal = false);
  std::deque<CommandGenerator> &channel2VoltageDiv(bool internal = false);
  std::deque<CommandGenerator> &levels(bool internal = false);
  std::deque<CommandGenerator> &pullSamples(bool internal = false);
  std::deque<CommandGenerator> &readEEROMandFPGA(bool internal = false);
  std::deque<CommandGenerator> &updateTimeBase(bool internal = false);
  std::deque<CommandGenerator> &trigger(bool internal = false);
  std::deque<CommandGenerator> &triggerSource(bool internal = false);
  std::deque<CommandGenerator> &voltageDiv(bool internal = false);
  std::deque<CommandGenerator> &updateXTriggerPos(bool internal = false);

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