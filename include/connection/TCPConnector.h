#ifndef _TCP_CONNECTOR_H_
#define _TCP_CONNECTOR_H_

#include "Connector.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

class TCPConnector : public Connector
{
public:
  TCPConnector(string host, int port);
  ~TCPConnector();

  void transmit(uint8_t *data, size_t length);
  void receive();

  void start();
  void stop();

  ConnectorType getType();

private:
  int socketHandle;
  sockaddr_in serverAddress;

  string host;
  int port;
};

#endif // _TCP_CONNECTOR_H_