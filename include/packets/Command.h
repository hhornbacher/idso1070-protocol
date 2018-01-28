#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "base.h"

#include "Response.h"

class Command
{
public:
  typedef function<void(void)> ResponseHandler;

  Command(CommandCode cmd, uint8_t param1 = 0, uint8_t param2 = 0);
  Command(uint8_t *payload);

  uint8_t *getPayload();

  void callResponseHandler();

  void setResponseHandler(ResponseHandler responseHandler);

  template <class S, class F>
  void setResponseHandler(S *self, F &&f)
  {
    setResponseHandler(bind(f, self, placeholders::_1));
  }

private:
  uint8_t payload[4];

  ResponseHandler responseHandler;
};

#endif // _COMMAND_H_