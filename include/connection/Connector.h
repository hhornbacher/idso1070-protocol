#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "base.h"
#include "packets/Response.h"

#include <queue>
#include <stdexcept>
#include <boost/circular_buffer.hpp>

class Connector
{
public:
  static const int RawBufferLength = 1024 * 64;

  class Exception : public runtime_error
  {
  public:
    Exception(string reason)
        : runtime_error("connection error"), reason(reason)
    {
    }

    virtual const char *what() const throw()
    {
      return reason.c_str();
    }

  private:
    string reason;
  };

  Connector();

  Response *getLatestResponse();
  size_t getResponseBufferSize();

  virtual void transmit(uint8_t *data, size_t length) = 0;
  virtual void receive() = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  virtual ConnectorType getType() = 0;

  bool isConnected();

protected:
  void grabPacket();

  boost::circular_buffer<uint8_t> rawBuffer;
  queue<Response *> responseBuffer;
  bool connected = false;
};

#endif // _CONNECTOR_H_