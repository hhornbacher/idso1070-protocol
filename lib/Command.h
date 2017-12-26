#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>
#include <cstring>

#include <deque>

#include "enums.h"

class Command
{
private:
  uint8_t payload[4];
  uint8_t responseCount = 1;

public:
  Command(CommandCode cmd);
  Command(uint8_t *cmd);
  Command(uint8_t *cmd, uint8_t responseCount);

  uint8_t *getPayload();
  uint8_t getResponseCount();
};

class CommandQueue : private std::deque<Command *>
{
private:
  size_t totalResponseCount = 0;

public:
  void add(Command *cmd);
  void add(CommandQueue cmds);

  size_t getTotalResponseCount();
  size_t getSize();
  Command *getNext();
};
// typedef std::deque<Command *> CommandQueue;

#endif // _COMMAND_H_