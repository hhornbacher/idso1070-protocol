#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <functional>

#include "../enums.h"

typedef std::function<bool(uint8_t *, uint8_t *, bool)> CommandResponseHandler;

class Command
{
public:
private:
  char name[256];
  uint8_t payload[4];
  CommandResponseHandler handler;
  bool inProgress = false;

public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  template <class F, class S>
  static CommandResponseHandler bindHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  }

  void setCommandHandler(CommandResponseHandler handler);
  bool callCommandHandler(uint8_t *responsePayload, bool success);

  uint8_t *getPayload();
  void setName(const char *name);

  bool isInProgress();
  void beginProgress();

  void print();
};

#endif // _COMMAND_H_