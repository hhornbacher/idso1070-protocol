#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <functional>

#include "enums.h"

class Command
{
public:
  typedef std::function<bool(uint8_t *cmdPayload, uint8_t *responsePayload, bool success)> Handler;

private:
  char name[256];
  uint8_t payload[4];
  Handler *handler = NULL;
  bool inProgress = false;

public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  void setHandler(Handler &handler);
  bool callHandler(uint8_t *responsePayload, bool success);

  uint8_t *getPayload();
  void setName(const char *name);

  bool isInProgress();
  void beginProgress();

  void print();
};

#endif // _COMMAND_H_