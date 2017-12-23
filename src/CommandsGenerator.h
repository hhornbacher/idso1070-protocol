#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include "Command.h"

class CommandsGenerator
{
private:
public:
  Command *readEEROMPage(uint8_t address);
  Command *readFPGAVersion();
  Command *keepAlive();
  Command *getSampleRate();
  Command *getfreqDivLowBytes();
  Command *getfreqDivHighBytes();
  CommandQueue readEEROM();
  CommandQueue getTimebase();
};

#endif // _COMMANDS_GENERATOR_H_