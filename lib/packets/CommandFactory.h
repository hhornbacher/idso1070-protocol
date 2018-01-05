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
  CommandResponseHandler nullHandler = [](uint8_t *, uint8_t *, bool success) { return success; };

public:
  CommandFactory(IDSO1070A &device);

  std::deque<CommandGenerator> &init(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &channelStatusOnly(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &channelStatus(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &channel1VoltageDiv(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &channel2VoltageDiv(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &levels(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &pullSamples(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &readEEROMandFPGA(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &updateTimeBase(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &trigger(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &triggerSource(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &voltageDiv(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerator> &updateXTriggerPos(CommandResponseHandler handler, bool internal = false);

  CommandGenerator selectChannel(CommandResponseHandler handler);
  CommandGenerator selectRAMChannel(CommandResponseHandler handler);
  CommandGenerator readFPGAVersion(CommandResponseHandler handler);
  CommandGenerator readBatteryLevel(CommandResponseHandler handler);
  CommandGenerator readRamCount(CommandResponseHandler handler);
  CommandGenerator getFreqDivLowBytes(CommandResponseHandler handler);
  CommandGenerator getFreqDivHighBytes(CommandResponseHandler handler);
  CommandGenerator updateSampleRate(CommandResponseHandler handler);
  CommandGenerator updateTriggerSourceAndSlope(CommandResponseHandler handler);
  CommandGenerator updateTriggerLevel(CommandResponseHandler handler);
  CommandGenerator updateChannelVolts125(CommandResponseHandler handler);
  CommandGenerator updateTriggerMode(CommandResponseHandler handler);
  CommandGenerator relay1(CommandResponseHandler handler);
  CommandGenerator relay2(CommandResponseHandler handler);
  CommandGenerator relay3(CommandResponseHandler handler);
  CommandGenerator relay4(CommandResponseHandler handler);
  CommandGenerator channel1Level(CommandResponseHandler handler);
  CommandGenerator channel2Level(CommandResponseHandler handler);
  CommandGenerator channel1Coupling(CommandResponseHandler handler);
  CommandGenerator channel2Coupling(CommandResponseHandler handler);
  CommandGenerator preTrigger(CommandResponseHandler handler);
  CommandGenerator postTrigger(CommandResponseHandler handler);
  CommandGenerator startSampling(CommandResponseHandler handler);
  CommandGenerator readEEROMPage(CommandResponseHandler handler, uint8_t address);
  CommandGenerator updateTriggerPWM(CommandResponseHandler handler, uint16_t pwm);
  CommandGenerator channel1PWM(CommandResponseHandler handler, uint16_t pwm);
  CommandGenerator channel2PWM(CommandResponseHandler handler, uint16_t pwm);
};

#endif // _COMMANDS_GENERATOR_H_