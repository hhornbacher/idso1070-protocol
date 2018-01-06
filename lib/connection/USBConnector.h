#ifndef _USB_CONNECTOR_H_
#define _USB_CONNECTOR_H_

#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "Connector.h"

#define USB_SERIAL_SPEED B4000000

class USBConnector : public Connector
{
private:
  const uint16_t idVendor = 0x0483;
  const uint16_t idProduct = 0x5740;

  char device[256];
  int handle;

public:
  USBConnector(const char *device);
  ~USBConnector();

  void transmit(uint8_t *data, size_t length);
  size_t receive();

  void start();
  void stop();
};

#endif // _USB_CONNECTOR_H_