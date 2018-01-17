#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include <cstdio>

#include <functional>
#include <deque>

#include "../util/mapValue.h"
#include "../device/IDSO1070.h"

#include "Command.h"

typedef std::function<Command *()> CommandGenerator;
typedef std::deque<CommandGenerator> CommandGeneratorVector;

class CommandFactory
{
public:
private:
  IDSO1070 &device;
  CommandGeneratorVector buffer;

public:
  CommandFactory(IDSO1070 &device);

  CommandGeneratorVector &readEEROMPages(bool internal = false);

  CommandGenerator readEEROMPage(uint8_t address);
  CommandGenerator readARMVersion();
  CommandGenerator readFPGAVersion();
  CommandGenerator readBatteryLevel();
  CommandGenerator readRamCount();
  CommandGenerator updateChannelSelection();
  CommandGenerator updateRAMChannelSelection();
  CommandGenerator updateFreqDivLowBytes();
  CommandGenerator updateFreqDivHighBytes();
  CommandGenerator updateSampleRate();
  CommandGenerator updateTriggerSourceAndSlope();
  CommandGenerator updateTriggerLevel();
  CommandGenerator updateChannelVolts125();
  CommandGenerator updateTriggerMode();
  CommandGenerator updateRelay1();
  CommandGenerator updateRelay2();
  CommandGenerator updateRelay3();
  CommandGenerator updateRelay4();
  CommandGenerator updateChannel1Level();
  CommandGenerator updateChannel2Level();
  CommandGenerator updateChannel1Coupling();
  CommandGenerator updateChannel2Coupling();
  CommandGenerator updatePreTriggerLength();
  CommandGenerator updatePostTriggerLength();
  CommandGenerator startSampling();
};

#endif // _COMMANDS_GENERATOR_H_