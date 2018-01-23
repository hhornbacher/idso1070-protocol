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
public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  uint8_t *getPayload();

private:
  uint8_t payload[4];
};

#endif // _COMMAND_H_