#ifndef _USB_CONNECTOR_H_
#define _USB_CONNECTOR_H_

#include "Connector.h"

class USBConnector : public Connector
{
  private:
    int socketHandle;
    sockaddr_in serverAddress;

  public:
    USBConnector(char *device);
    ~USBConnector();

    void transmit(uint8_t *data, size_t length);
    size_t receive();

    void start();
    void stop();
};

#endif // _USB_CONNECTOR_H_