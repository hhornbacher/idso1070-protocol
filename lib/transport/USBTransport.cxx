#include <transport/USBTransport.h>

USBTransport::USBTransport(std::string device) : port_(ioService_), device_(device)
{
}

void USBTransport::setupConnection()
{
  boost::system::error_code ec;
  port_.open(device_.c_str(), ec);

  if (ec)
  {
    Exception e(ec.message());
    connectionHandler_(&e, false);
    connected_ = false;
    return;
  }

  port_.set_option(boost::asio::serial_port_base::baud_rate(4000000));
  port_.set_option(boost::asio::serial_port_base::character_size(8));
  port_.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
  port_.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
  port_.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

  ioServiceThread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, &ioService_));
  connectionHandler_(NULL, true);
  connected_ = true;
}

void USBTransport::stopConnection()
{
  port_.cancel();
  port_.close();
  ioService_.stop();
  ioService_.reset();
  connectionHandler_(NULL, false);
  connected_ = false;
  delete ioServiceThread_;
}

void USBTransport::send()
{
  throttlingTimer_.async_wait([this](const boost::system::error_code &ec) {
    // boost::asio::async_write(
    //     port_,
    //     requestPtr_->getBuffer(),
    //     [this](const boost::system::error_code &ec, std::size_t bytes_transferred) {
    //       if (ec)
    //       {
    //             connectionHandler_(ec, false);
    //           connected_=false;
    //         return;
    //       }
    //       state_ = TransportReceive;
    //     });

    throttlingTimer_.expires_from_now(boost::posix_time::milliseconds(150));
  });
}

void USBTransport::receive()
{
  timeoutTimer_.expires_from_now(boost::posix_time::milliseconds(timeout_));
  timeoutTimer_.async_wait([this](const boost::system::error_code &ec) {
    port_.cancel();

    Exception e("Timeout");
    transactionHandler_(&e);
  });
  // boost::asio::async_read(
  //     port_,
  //     responseBuffer,
  //     boost::asio::transfer_at_least(Response::Size),
  //     [this](const boost::system::error_code &ec, std::size_t bytes_transferred) {
  //       if (ec)
  //       {
  //         connectionHandler_(ec, false);
  //           connected_=false;
  //         return;
  //       }
  //       transactionHandler_();
  //     });
}
