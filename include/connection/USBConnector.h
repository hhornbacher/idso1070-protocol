#ifndef _USB_CONNECTOR_H_
#define _USB_CONNECTOR_H_

#include "Connector.h"

#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <termios.h>
#include <list>

using namespace std;

class USBConnector : public Connector
{
public:
  static const int USBSerialSpeed = B4000000;

  typedef list<string> USBDeviceList;

  USBConnector(string device);
  ~USBConnector();

  void transmit(uint8_t *data, size_t length);
  size_t receive();

  static void enumerateDevices(USBDeviceList &list);

  void start();
  void stop();

  ConnectorType getType();

private:
  string device;
  int handle;
};

#endif // _USB_CONNECTOR_H_