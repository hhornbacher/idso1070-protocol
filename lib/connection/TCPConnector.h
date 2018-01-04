#ifndef _TCP_CONNECTOR_H_
#define _TCP_CONNECTOR_H_

#include "Connector.h"

class TCPConnector : public Connector
{
  private:
    int socketHandle;
    sockaddr_in serverAddress;

  public:
    TCPConnector(char *host, int port);
    ~TCPConnector();

    void transmit(uint8_t *data, size_t length);
    size_t receive();

    void start();
    void stop();
};

#endif // _TCP_CONNECTOR_H_