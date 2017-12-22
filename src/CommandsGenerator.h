#ifndef _COMMANDS_GENERATOR_H_
#define _COMMANDS_GENERATOR_H_

#include "Command.h"

class CommandsGenerator
{
  private:
  public:
    Command *readEEROMPage(uint8_t address);
    CommandQueue readEEROM();
};

#endif // _COMMANDS_GENERATOR_H_