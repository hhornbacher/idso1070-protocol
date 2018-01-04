#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include <cstdio>

#include <functional>
#include <deque>

#include "../util/mapValue.h"
#include "../device/IDSO1070A.h"
#include "Command.h"

typedef std::function<Command *()> CommandGenerationFunction;

class CommandGenerator
{
private:
  IDSO1070A &device;
  std::deque<CommandGenerationFunction> buffer;
  CommandResponseHandler nullHandler = [](uint8_t *, uint8_t *, bool) { return true; };

public:
  CommandGenerator(IDSO1070A &device);

  std::deque<CommandGenerationFunction> &init(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &channelStatusOnly(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &channelStatus(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &channel1VoltageDiv(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &channel2VoltageDiv(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &levels(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &pullSamples(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &readEEROMandFPGA(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &updateTimeBase(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &trigger(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &triggerSource(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &voltageDiv(CommandResponseHandler handler, bool internal = false);
  std::deque<CommandGenerationFunction> &updateXTriggerPos(CommandResponseHandler handler, bool internal = false);

  CommandGenerationFunction selectChannel(CommandResponseHandler handler);
  CommandGenerationFunction selectRAMChannel(CommandResponseHandler handler);
  CommandGenerationFunction readFPGAVersion(CommandResponseHandler handler);
  CommandGenerationFunction readBatteryLevel(CommandResponseHandler handler);
  CommandGenerationFunction readRamCount(CommandResponseHandler handler);
  CommandGenerationFunction getFreqDivLowBytes(CommandResponseHandler handler);
  CommandGenerationFunction getFreqDivHighBytes(CommandResponseHandler handler);
  CommandGenerationFunction updateSampleRate(CommandResponseHandler handler);
  CommandGenerationFunction updateTriggerSourceAndSlope(CommandResponseHandler handler);
  CommandGenerationFunction updateTriggerLevel(CommandResponseHandler handler);
  CommandGenerationFunction updateChannelVolts125(CommandResponseHandler handler);
  CommandGenerationFunction updateTriggerMode(CommandResponseHandler handler);
  CommandGenerationFunction relay1(CommandResponseHandler handler);
  CommandGenerationFunction relay2(CommandResponseHandler handler);
  CommandGenerationFunction relay3(CommandResponseHandler handler);
  CommandGenerationFunction relay4(CommandResponseHandler handler);
  CommandGenerationFunction channel1Level(CommandResponseHandler handler);
  CommandGenerationFunction channel2Level(CommandResponseHandler handler);
  CommandGenerationFunction channel1Coupling(CommandResponseHandler handler);
  CommandGenerationFunction channel2Coupling(CommandResponseHandler handler);
  CommandGenerationFunction preTrigger(CommandResponseHandler handler);
  CommandGenerationFunction postTrigger(CommandResponseHandler handler);
  CommandGenerationFunction startSampling(CommandResponseHandler handler);
  CommandGenerationFunction readEEROMPage(CommandResponseHandler handler, uint8_t address);
  CommandGenerationFunction updateTriggerPWM(CommandResponseHandler handler, uint16_t pwm);
  CommandGenerationFunction channel1PWM(CommandResponseHandler handler, uint16_t pwm);
  CommandGenerationFunction channel2PWM(CommandResponseHandler handler, uint16_t pwm);
};

#endif // _COMMANDS_GENERATOR_H_