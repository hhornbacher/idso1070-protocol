#ifndef _USB_CONNECTOR_H_
#define _USB_CONNECTOR_H_

#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <termios.h>
#include <list>

#include "connection/Connector.h"

using namespace std;

class USBConnector : public Connector
{
public:
  static const int USBSerialSpeed = B4000000;

  typedef list<string> USBDeviceList;

private:
  char device[256];
  int handle;

public:
  USBConnector(const char *device);
  ~USBConnector();

  void transmit(uint8_t *data, size_t length);
  size_t receive();

  static void enumerateDevices(USBDeviceList &list);

  void start();
  void stop();
};

#endif // _USB_CONNECTOR_H_