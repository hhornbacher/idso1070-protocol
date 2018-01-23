#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <queue>
#include <stdexcept>
#include <boost/circular_buffer.hpp>

#include "packets/Response.h"

using namespace std;

class ConnectException : public runtime_error
{
public:
  ConnectException(string description)
      : runtime_error("connection failed"), description(description)
  {
  }

  virtual const char *what() const throw()
  {
    return description.c_str();
  }

private:
  string description;
};

enum ConnectorType
{
  CONNECTOR_USB,
  CONNECTOR_WIFI
};

class Connector
{
public:
  static const int RawBufferLength = 1024 * 64;

  Connector();

  Response *getLatestResponse();
  size_t getResponseBufferSize();

  virtual void transmit(uint8_t *data, size_t length) = 0;
  virtual size_t receive() = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  virtual ConnectorType getType() = 0;

  bool isConnected();

protected:
  void grabPacket();

  boost::circular_buffer<uint8_t> rawBuffer;
  std::queue<Response *> responseBuffer;
  bool connected = false;
};

#endif // _CONNECTOR_H_