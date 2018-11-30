#include <Protocol.h>

#include <util.h>
#include <iostream>

Protocol::Protocol(const char *device) : port_(ioService_), device_(device)
{
}

void Protocol::start()
{
  boost::system::error_code ec;
  printf("Connecting\n");
  port_.open(device_.c_str(), ec);

  if (ec)
  {
    throw Exception(ec.message());
  }
  port_.set_option(boost::asio::serial_port_base::baud_rate(4000000));
  port_.set_option(boost::asio::serial_port_base::character_size(8));
  port_.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
  port_.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
  port_.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

  boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService_));
}

void Protocol::stop()
{
  boost::mutex::scoped_lock look(mutex_);
  port_.cancel();
  port_.close();
  ioService_.stop();
  ioService_.reset();
}

void Protocol::sendRequest(Request &request, Response &response)
{
  printf("Sending request\n");
  boost::system::error_code ec;
  boost::asio::streambuf responseBuffer;

  printf("%s", request.toString().c_str());

  boost::asio::write(port_, request.getBuffer(), ec);
  if (ec)
  {
    throw Exception(ec.message());
  }

  printf("Receiving...\n");
  boost::asio::read(port_, responseBuffer, boost::asio::transfer_at_least(509), ec);
  if (ec)
  {
    throw Exception(ec.message());
  }

  response.setData(responseBuffer);

  if (response.isValid())
  {
    printf("Valid response!\n");
  }
  else
  {
    printf("Invalid response!\n");
  }
  printf("%s", response.toString().c_str());
}
