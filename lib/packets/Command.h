#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <functional>

#include "../enums.h"
#include "Response.h"

class Command
{
private:
  char name[256];
  uint8_t payload[4];
  bool inProgress = false;

public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  uint8_t *getPayload();
  void setName(const char *name);

  bool isInProgress();
  void beginProgress();

  void print();
};

#endif // _COMMAND_H_