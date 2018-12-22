#pragma once

#include "../Exception.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <functional>

class Transport
{
public:
  typedef std::function<void(Exception *, bool)> ConnectionHandler;
  typedef std::function<void(Exception *)> TransactionHandler;

  Transport();

  void connect(ConnectionHandler connectionHandler);
  void disconnect(ConnectionHandler connectionHandler);

  // void process();

  void transaction(uint8_t *txBuffer, size_t txBufferLength, uint8_t *rxBuffer, size_t rxBufferLength, TransactionHandler transactionHandler);
  void receive(uint8_t *rxBuffer, size_t rxBufferLength, TransactionHandler transactionHandler);

protected:
  virtual void setupConnection() = 0;
  virtual void stopConnection() = 0;
  virtual void send() = 0;
  virtual void receive() = 0;

  boost::mutex mutex_;
  boost::asio::io_service ioService_;
  boost::thread *ioServiceThread_{NULL};
  boost::asio::deadline_timer throttlingTimer_;
  boost::asio::deadline_timer timeoutTimer_;

  ConnectionHandler connectionHandler_;
  TransactionHandler transactionHandler_;

  bool busy_{false};
  bool connected_{false};

  uint8_t *txBuffer_{NULL};
  size_t txBufferLength_{0};
  uint8_t *rxBuffer_{NULL};
  size_t rxBufferLength_{0};

  uint32_t timeout_{1000};
};