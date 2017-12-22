#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PACKETBUFFER_LENGTH 1024 * 4

class Connector
{
private:
  int socketHandle;
  sockaddr_in serverAddress;

  uint8_t packetBuffer[PACKETBUFFER_LENGTH];
  size_t packetBufferLength = 0;

public:
  Connector(char *host, int port);
  ~Connector();

  void transmit(uint8_t *data, size_t length);
  size_t receive();

  uint8_t *getPacketBuffer();
  size_t getPacketBufferLength();
  void clearPacketBuffer();

  void start();
  void stop();
};

#endif // _CONNECTOR_H_