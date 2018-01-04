#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define PACKETBUFFER_LENGTH 1024 * 4

class Connector
{
protected:
  uint8_t packetBuffer[PACKETBUFFER_LENGTH];
  size_t packetBufferLength = 0;
  bool usbConnection;

public:
  virtual void transmit(uint8_t *data, size_t length) = 0;
  virtual size_t receive() = 0;

  virtual void start() = 0;
  virtual void stop() = 0;

  uint8_t *getPacketBuffer();
  size_t getPacketBufferLength();
  void clearPacketBuffer();

  bool isUsbConnection();
};

#endif // _CONNECTOR_H_