#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <deque>

#include "enums.h"

class Command
{
private:
  char name[256];
  uint8_t payload[4];
  uint8_t responseCount = 1;

public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  uint8_t *getPayload();
  uint8_t getResponseCount();
  uint8_t setResponseCount(uint8_t responseCount);
  void setName(char *name);

  void print();
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