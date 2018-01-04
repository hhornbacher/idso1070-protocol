#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <functional>

#include "../enums.h"

class Command
{
public:
  typedef std::function<bool(uint8_t *, uint8_t *, bool)> Handler;

private:
  char name[256];
  uint8_t payload[4];
  Handler handler;
  bool inProgress = false;

public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  void setHandler(Handler handler);
  template <class F, class S>
  void setHandler(F &&f, S *self)
  {
    handler = std::bind(f, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  }

  bool callHandler(uint8_t *responsePayload, bool success);

  uint8_t *getPayload();
  void setName(const char *name);

  bool isInProgress();
  void beginProgress();

  void print();
};

#endif // _COMMAND_H_