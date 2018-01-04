#ifndef _TEST_CONNECTOR_H_
#define _TEST_CONNECTOR_H_

#include "connection/Connector.h"
#include "packets/ResponsePacket.h"

class TestConnector : public Connector
{
private:
  bool pending = false;

public:
  TestConnector();
  ~TestConnector();

  void transmit(uint8_t *data, size_t length);
  size_t receive();

  void start();
  void stop();

  void fakePacket(uint8_t *data);
};

#endif // _TEST_CONNECTOR_H_