#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <queue>

#include "../packets/Response.h"

#define RAW_BUFFER_LENGTH 1024 * 32

class Connector
{
protected:
  uint8_t rawBuffer[RAW_BUFFER_LENGTH];
  size_t rawBufferLength = 0;
  bool usbConnection;

  std::queue<Response *> responseBuffer;

  void grabPacket();

public:
  virtual void transmit(uint8_t *data, size_t length) = 0;
  virtual size_t receive() = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  Response *getLatestResponse();
  size_t getResponseBufferSize();
  // void clearPacketBuffer();

  bool isUsbConnection();
};

#endif // _CONNECTOR_H_