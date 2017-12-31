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

public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  uint8_t *getPayload();
  void setName(const char *name);

  void print();
};

class CommandQueue : private std::deque<Commands>
{
public:
  void addFront(Commands cmd);
  void add(Commands cmd);

  size_t getSize();
  Commands getNext();
};
// typedef std::deque<Command *> CommandQueue;

#endif // _COMMAND_H_