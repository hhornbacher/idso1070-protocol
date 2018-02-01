#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include "base.h"
#include "util.h"
#include "IDSO1070.h"
#include "Command.h"

class CommandFactory
{
public:
  Command *readEEPROMPage(uint8_t address);
  Command *readARMVersion();
  Command *readFPGAVersion();
  Command *readBatteryLevel();
  Command *readRamCount(int enabledChannels, uint16_t samplesNumberOfOneFrame,
                        bool isSampleRate200Mor250M, double triggerXPosition,
                        uint8_t packetsNumber);

  Command *startSampling();

  Command *updateSampleRate(TimeBase timeBase, int enabledChannels);
  Command *updateFreqDivLowBytes(TimeBase timeBase);
  Command *updateFreqDivHighBytes(TimeBase timeBase);
  Command *updateRAMChannelSelection(bool channel1Enabled, bool channel2Enabled);
  Command *updateChannelVolts125(VoltageDiv channel1VerticalDiv, VoltageDiv channel2VerticalDiv);
  Command *updateRelay1(VoltageDiv channel1VerticalDiv);
  Command *updateRelay2(VoltageDiv channel1VerticalDiv);
  Command *updateRelay3(VoltageDiv channel2VerticalDiv);
  Command *updateRelay4(VoltageDiv channel2VerticalDiv);
  Command *updateChannel1Coupling(InputCoupling channel1Coupling);
  Command *updateChannel2Coupling(InputCoupling channel2Coupling);
  Command *updateTriggerMode(CaptureMode capMode, TriggerMode trigMode, ScopeMode scoMode);
  Command *updateChannel1Level(VoltageDiv channel1VerticalDiv, int16_t channel1VerticalPosition, float channel1PWM0, float channel1PWM1);
  Command *updateChannel2Level(VoltageDiv channel2VerticalDiv, int16_t channel2VerticalPosition, float channel2PWM0, float channel2PWM1);
  Command *updateTriggerSourceAndSlope(TriggerChannel triggerChanel, ScopeMode scoMode, TriggerSlope triggerSlope);
  Command *updateTriggerLevel(uint16_t triggerLevel, float triggerTopPWM, float triggerBottomPWM);
  Command *updateChannelSelection(bool channel1Enabled, bool channel2Enabled, bool isSampleRate200Mor250M);
  Command *updatePreTriggerLength(uint16_t samplesNumberOfOneFrame, double triggerXPosition);
  Command *updatePostTriggerLength(uint16_t samplesNumberOfOneFrame, double triggerXPosition);
};

#endif // _COMMANDS_GENERATOR_H_