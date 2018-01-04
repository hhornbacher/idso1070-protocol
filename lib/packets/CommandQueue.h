#ifndef _COMMAND_QUEUE_H_
#define _COMMAND_QUEUE_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <deque>

#include "Command.h"

class CommandQueue : private std::deque<Command *>
{
public:
  void add(Command *cmd);

  size_t getSize();

  Command &getCurrent();
  void destroyCurrent();
};

#endif // _COMMAND_QUEUE_H_