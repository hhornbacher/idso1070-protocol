#include <transport/Transport.h>

Transport::Transport() : StateMachine(Disconnected), ioServiceThread_(nullptr),
                         worker_(ioService_),
                         throttlingTimer_(ioService_, boost::posix_time::milliseconds(150)),
                         timeoutTimer_(ioService_, boost::posix_time::milliseconds(0)),
                         requestPtr_(nullptr)
{
  registerState(Connecting, std::bind(&Transport::stateConnecting, this, std::placeholders::_1));
  registerState(Connected, std::bind(&Transport::stateConnected, this, std::placeholders::_1));
  registerState(Disconnecting, std::bind(&Transport::stateDisconnecting, this, std::placeholders::_1));
  registerState(Disconnected, std::bind(&Transport::stateDisconnected, this, std::placeholders::_1));
  registerState(Throttling, std::bind(&Transport::stateThrottling, this, std::placeholders::_1));
  registerState(Writing, std::bind(&Transport::stateWriting, this, std::placeholders::_1));
  registerState(Reading, std::bind(&Transport::stateReading, this, std::placeholders::_1));
  registerTransition(Disconnecting, Disconnected);
  registerTransition(Disconnected, Connecting);
  registerTransition(Connecting, Connected);
  registerTransition(Connecting, Disconnected);
  registerTransition(Connected, Disconnecting);
  registerTransition(Connected, Throttling);
  registerTransition(Throttling, Writing);
  registerTransition(Writing, Reading);
  registerTransition(Reading, Connected);
}

Transport::~Transport()
{
}

void Transport::start()
{
  // Spawn state machine thread
  StateMachine::start();

  // Spawn io_service thread, to start processing events
  ioServiceThread_ = std::make_unique<boost::thread>(boost::bind(&boost::asio::io_service::run, &ioService_));
}

void Transport::stop()
{
  // Stop state machine thread
  StateMachine::stop();

  // Stop & reset io_service (thread)
  ioService_.stop();
  ioService_.reset();

  // Unset thread pointer
  ioServiceThread_ = nullptr;
}

void Transport::connect(ConnectionHandler connectionHandler)
{
  if (externalEvent(Connecting))
  {
    connectionHandler_ = connectionHandler;
    commitExternalEvent();
  }
}

void Transport::disconnect(ConnectionHandler connectionHandler)
{
  if (externalEvent(Disconnecting))
  {
    connectionHandler_ = connectionHandler;
    commitExternalEvent();
  }
}

void Transport::request(const std::shared_ptr<Request> requestPtr, RequestHandler requestHandler, bool stream)
{
  if (externalEvent(Throttling))
  {
    requestPtr_ = requestPtr;
    requestHandler_ = requestHandler;
    stream_ = stream;
    commitExternalEvent();
  }
}

void Transport::internalConnectHandler(const boost::system::error_code &ec)
{
  if (ec)
  {
    printf("internalConnectHandler: Error: %s\n", ec.message().c_str());
    ec_ = ec;
    internalEvent(Disconnected);
    return;
  }
  internalEvent(Connected);
}

void Transport::internalDisconnectHandler(const boost::system::error_code &ec)
{
  if (ec)
  {
    printf("internalDisconnectHandler: Error: %s\n", ec.message().c_str());
    ec_ = ec;
    internalEvent(Connected);
    return;
  }
  internalEvent(Disconnected);
}

void Transport::internalWriteHandler(const boost::system::error_code &ec, std::size_t bytesTransferred)
{
  if (ec)
  {
    printf("internalWriteHandler: Error: %s\n", ec.message().c_str());
    ec_ = ec;
    internalEvent(Disconnected);
    return;
  }
  internalEvent(Reading);
}

void Transport::internalReadHandler(const boost::system::error_code &ec, std::size_t bytesTransferred)
{
  if (ec)
  {
    printf("internalReadHandler: Error: %s\n", ec.message().c_str());
    ec_ = ec;
    internalEvent(Disconnected);
    return;
  }
  internalEvent(Connected);
}

void Transport::internalThrottlingHandler(const boost::system::error_code &ec)
{
  if (ec)
  {
    printf("internalThrottlingHandler: Error: %s\n", ec.message().c_str());
    ec_ = ec;
    internalEvent(Connected);
    return;
  }
  internalEvent(Writing);
}

void Transport::internalTimeoutHandler(const boost::system::error_code &ec)
{
  if (ec)
  {
    printf("internalTimeoutHandler: Error: %s\n", ec.message().c_str());
    ec_ = ec;
    internalEvent(Connected);
    return;
  }

  ec_ = boost::system::errc::make_error_code(boost::system::errc::timed_out);
  internalEvent(Connected);
}

void Transport::stateConnecting(StateMachine::Phase phase)
{
  if (phase == StateMachine::Phase::Do)
    return;
  else if (phase == StateMachine::Phase::Entry)
  {
    connectImpl();
  }
  else if (phase == StateMachine::Phase::Exit)
  {
    if (ec_)
    {
      connectionHandler_(std::make_shared<Error>(ec_.message()), false);
      ec_ = boost::system::errc::make_error_code(boost::system::errc::success);
    }
    else
    {
      connectionHandler_(nullptr, true);
    }
  }
}

void Transport::stateConnected(StateMachine::Phase phase)
{
}

void Transport::stateDisconnecting(StateMachine::Phase phase)
{
  if (phase == StateMachine::Phase::Do)
    return;
  else if (phase == StateMachine::Phase::Entry)
  {
    disconnectImpl();
  }
  else if (phase == StateMachine::Phase::Exit)
  {
    if (ec_)
    {
      connectionHandler_(std::make_shared<Error>(ec_.message()), true);
      ec_ = boost::system::errc::make_error_code(boost::system::errc::success);
    }
    else
    {
      connectionHandler_(nullptr, false);
    }
  }
}

void Transport::stateDisconnected(StateMachine::Phase phase)
{
}

void Transport::stateThrottling(StateMachine::Phase phase)
{
  if (phase == StateMachine::Phase::Do)
    return;
  else if (phase == StateMachine::Phase::Entry)
  {
    throttlingTimer_.async_wait(std::bind(&Transport::internalThrottlingHandler, this, std::placeholders::_1));
  }
  else if (phase == StateMachine::Phase::Exit)
  {
    if (ec_)
    {
      requestHandler_(std::make_shared<Error>(ec_.message()), nullptr);
      ec_ = boost::system::errc::make_error_code(boost::system::errc::success);
    }
  }
}

void Transport::stateWriting(StateMachine::Phase phase)
{
  if (phase == StateMachine::Phase::Do)
    return;
  else if (phase == StateMachine::Phase::Entry)
  {
    writeImpl();
  }
  else if (phase == StateMachine::Phase::Exit)
  {
    if (ec_)
    {
      requestHandler_(std::make_shared<Error>(ec_.message()), nullptr);
      ec_ = boost::system::errc::make_error_code(boost::system::errc::success);
    }
    else
    {
      requestHandler_(nullptr, false);
      throttlingTimer_.expires_from_now(boost::posix_time::milliseconds(150));
    }
  }
}

void Transport::stateReading(StateMachine::Phase phase)
{
  if (phase == StateMachine::Phase::Do)
    return;
  else if (phase == StateMachine::Phase::Entry)
  {
    timeoutTimer_.expires_from_now(boost::posix_time::milliseconds(RequestTimeout));
    timeoutTimer_.async_wait(std::bind(&Transport::internalTimeoutHandler, this, std::placeholders::_1));
    readImpl();
  }
  else if (phase == StateMachine::Phase::Exit)
  {
    if (ec_)
    {
      cancelImpl();
      requestHandler_(std::make_shared<Error>(ec_.message()), nullptr);
      ec_ = boost::system::errc::make_error_code(boost::system::errc::success);
    }
    else
    {
      timeoutTimer_.cancel();
      requestHandler_(nullptr, responsePtr_);
    }
  }
}
