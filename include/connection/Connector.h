#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <queue>

#include <boost/circular_buffer.hpp>

#include "../packets/Response.h"

class Connector
{
public:
  static const int RawBufferLength = 1024 * 4;

protected:
  boost::circular_buffer<uint8_t> rawBuffer;
  bool usbConnection;

  std::queue<Response *> responseBuffer;

  void grabPacket();

public:
  Connector();

  virtual void transmit(uint8_t *data, size_t length) = 0;
  virtual size_t receive() = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  Response *getLatestResponse();
  size_t getResponseBufferSize();

  bool isUsbConnection();
};

#endif // _CONNECTOR_H_