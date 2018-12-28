#pragma once

#include "../Error.h"
#include "../StateMachine.h"
#include "../packet/Request.h"
#include "../packet/Response.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <functional>

class Transport : public StateMachine
{
public:
  static constexpr int RequestTimeout = 1000;
  typedef std::function<void(std::shared_ptr<Error>, bool)> ConnectionHandler;
  typedef std::function<void(std::shared_ptr<Error>, std::shared_ptr<Response>)> RequestHandler;

  Transport();
  virtual ~Transport() = 0;

  void start();
  void stop();

  void connect(ConnectionHandler connectionHandler);
  void disconnect(ConnectionHandler connectionHandler);

  void request(const std::shared_ptr<Request> requestPtr, RequestHandler requestHandler, bool stream = false);

protected:
  // These methods are called from the state machine thread
  virtual void connectImpl() = 0;
  virtual void disconnectImpl() = 0;
  virtual void readImpl() = 0;
  virtual void writeImpl() = 0;
  virtual void cancelImpl() = 0;

  // These methods are called from the state machine thread
  void stateConnecting(StateMachine::Phase phase);
  void stateConnected(StateMachine::Phase phase);
  void stateDisconnecting(StateMachine::Phase phase);
  void stateDisconnected(StateMachine::Phase phase);
  void stateThrottling(StateMachine::Phase phase);
  void stateWriting(StateMachine::Phase phase);
  void stateReading(StateMachine::Phase phase);

  // These methods are called from the io_service thread
  void internalConnectHandler(const boost::system::error_code &ec);
  void internalDisconnectHandler(const boost::system::error_code &ec);
  void internalWriteHandler(const boost::system::error_code &ec, std::size_t bytesTransferred);
  void internalReadHandler(const boost::system::error_code &ec, std::size_t bytesTransferred);
  void internalThrottlingHandler(const boost::system::error_code &ec);
  void internalTimeoutHandler(const boost::system::error_code &ec);

protected:
  enum States : StateMachine::StateID
  {
    Connecting,
    Connected,
    Disconnecting,
    Disconnected,
    Throttling,
    Writing,
    Reading
  };

  boost::asio::io_service ioService_;
  std::unique_ptr<boost::thread> ioServiceThread_;
  boost::asio::io_service::work worker_;
  boost::asio::deadline_timer throttlingTimer_;
  boost::asio::deadline_timer timeoutTimer_;
  boost::system::error_code ec_;

  ConnectionHandler connectionHandler_;
  RequestHandler requestHandler_;

  bool connected_{false};
  bool stream_{false};

  std::shared_ptr<Request> requestPtr_;
  std::shared_ptr<Response> responsePtr_;
};