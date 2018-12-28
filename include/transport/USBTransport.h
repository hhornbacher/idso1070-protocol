#pragma once

#include "Transport.h"

class USBTransport : public Transport
{
public:
  USBTransport(std::string device);

protected:
  void connectImpl();
  void disconnectImpl();
  void readImpl();
  void writeImpl();
  void cancelImpl();

private:
  std::string device_;
  boost::asio::serial_port port_;
};