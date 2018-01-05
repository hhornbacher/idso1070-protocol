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
  typedef std::function<bool(Command *, Response *, int)> ResponseHandler;

private:
  char name[256];
  uint8_t payload[4];
  ResponseHandler handler;
  bool inProgress = false;

public:
  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  template <class F, class S>
  static ResponseHandler bindHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  }

  void setHandler(ResponseHandler handler);
  bool callHandler(Response *response, int retries);

  uint8_t *getPayload();
  void setName(const char *name);

  bool isInProgress();
  void beginProgress();

  void print();
};

#endif // _COMMAND_H_