#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include <cstdio>

#include <functional>
#include <deque>

#include "../util/mapValue.h"
#include "../device/IDSO1070.h"

#include "Command.h"

typedef std::function<Command *()> CommandGenerator;

class CommandFactory
{
public:
  CommandGenerator readEEROMPage(uint8_t address);
  CommandGenerator readARMVersion();
  CommandGenerator readFPGAVersion();
  CommandGenerator readBatteryLevel();
  CommandGenerator readRamCount(int enabledChannels, uint16_t samplesNumberOfOneFrame,
                                bool isSampleRate200Mor250M, double triggerXPosition,
                                uint8_t packetsNumber);

  CommandGenerator startSampling();

  CommandGenerator updateSampleRate(TimeBase timeBase, int enabledChannels);
  CommandGenerator updateFreqDivLowBytes(TimeBase timeBase);
  CommandGenerator updateFreqDivHighBytes(TimeBase timeBase);
  CommandGenerator updateRAMChannelSelection(bool channel1Enabled, bool channel2Enabled);
  CommandGenerator updateChannelVolts125(VoltageDiv channel1VerticalDiv, VoltageDiv channel2VerticalDiv);
  CommandGenerator updateRelay1(VoltageDiv channel1VerticalDiv);
  CommandGenerator updateRelay2(VoltageDiv channel1VerticalDiv);
  CommandGenerator updateRelay3(VoltageDiv channel2VerticalDiv);
  CommandGenerator updateRelay4(VoltageDiv channel2VerticalDiv);
  CommandGenerator updateChannel1Coupling(InputCoupling channel1Coupling);
  CommandGenerator updateChannel2Coupling(InputCoupling channel2Coupling);
  CommandGenerator updateTriggerMode(CaptureMode capMode, TriggerMode trigMode, ScopeMode scoMode);
  CommandGenerator updateChannel1Level(VoltageDiv channel1VerticalDiv, int16_t channel1VerticalPosition, float channel1PWM0, float channel1PWM1);
  CommandGenerator updateChannel2Level(VoltageDiv channel2VerticalDiv, int16_t channel2VerticalPosition, float channel2PWM0, float channel2PWM1);
  CommandGenerator updateTriggerSourceAndSlope(TriggerChannel triggerChanel, ScopeMode scoMode, TriggerSlope triggerSlope);
  CommandGenerator updateTriggerLevel(uint16_t triggerLevel, float triggerTopPWM, float triggerBottomPWM);
  CommandGenerator updateChannelSelection(bool channel1Enabled, bool channel2Enabled, bool isSampleRate200Mor250M);
  CommandGenerator updatePreTriggerLength(uint16_t samplesNumberOfOneFrame, double triggerXPosition);
  CommandGenerator updatePostTriggerLength(uint16_t samplesNumberOfOneFrame, double triggerXPosition);
};

#endif // _COMMANDS_GENERATOR_H_