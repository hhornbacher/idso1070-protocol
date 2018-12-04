#pragma once

#include "packet/Request.h"
#include "packet/Response.h"
#include "SampleStream.h"

#include <stdexcept>
#include <deque>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class Protocol
{
public:
  static constexpr int MaxPWM = 4095;

  enum States
  {
    Idle,
    SendRequest,
    ReceiveResponse,
    Cancled,
    Sampling
  };

  Protocol(const char *device);

  void start();
  void stop();

  void request(Request *req);

protected:
  void stateMachine();

  boost::mutex mutex_;
  boost::asio::io_service ioService_;
  boost::asio::serial_port port_;

  std::string device_;

  bool running_{false};
  States state_{Idle};

  bool error_{false};
  std::string errorMessage_;

  boost::asio::deadline_timer requestTimer_;
  std::deque<std::shared_ptr<Request>> requestQueue_;

  boost::thread *ioServiceThread_;
  boost::thread *stateMachineThread_;
};