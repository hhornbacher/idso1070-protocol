#pragma once

#include "Transport.h"

class USBTransport : public Transport
{
public:
  USBTransport(std::string device);

protected:
  void setupConnection();
  void stopConnection();
  void send();
  void receive();

private:
  std::string device_;
  boost::asio::serial_port port_;
};