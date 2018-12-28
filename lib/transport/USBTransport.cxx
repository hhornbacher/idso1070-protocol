#include <transport/USBTransport.h>

#include <memory>
#include <utility>

USBTransport::USBTransport(std::string device) : device_(device), port_(ioService_)
{
}

void USBTransport::connectImpl()
{
  printf("connectImpl\n");
  boost::system::error_code ec;

  // Try to open serial port device
  port_.open(device_.c_str(), ec);

  // If there was an error, exit immediately
  if (ec)
  {
    internalConnectHandler(ec);
    return;
  }

  // Setup serial port configuration (4000000 baud, 8N1, no flow control)
  port_.set_option(boost::asio::serial_port_base::baud_rate(4000000));
  port_.set_option(boost::asio::serial_port_base::character_size(8));
  port_.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
  port_.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
  port_.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

  internalConnectHandler(ec);
}

void USBTransport::disconnectImpl()
{
  printf("disconnectImpl\n");

  // Cancel pending communications & stop serial port
  port_.cancel();
  port_.close();

  internalDisconnectHandler(boost::system::errc::make_error_code(boost::system::errc::success));
}

void USBTransport::readImpl()
{
  printf("readImpl\n");
  boost::asio::async_read(
      port_,
      boost::asio::buffer(responsePtr_->getData().data(), responsePtr_->getSize()),
      boost::asio::transfer_at_least(Response::Size),
      [this](const boost::system::error_code &ec, std::size_t bytesTransferred) {
        internalWriteHandler(ec, bytesTransferred);
      });
}

void USBTransport::writeImpl()
{
  printf("writeImpl\n");
  boost::asio::async_write(
      port_,
      boost::asio::buffer(requestPtr_->getData().data(), requestPtr_->getSize()),
      [this](const boost::system::error_code &ec, std::size_t bytesTransferred) {
        internalReadHandler(ec, bytesTransferred);
      });
}

void USBTransport::cancelImpl()
{
  printf("cancelImpl\n");
  port_.cancel();
}

// void USBTransport::request(std::shared_ptr<Request> requestPtr, RequestHandler requestHandler)
// {
//   if (connected_)
//   {
//     requestHandler_ = requestHandler;
//     printf("Send\n");
//     throttlingTimer_.async_wait([this, requestPtr](const boost::system::error_code &ec) {
//       boost::asio::async_write(
//           port_,
//           boost::asio::buffer(requestPtr->getData(), requestPtr->getSize()),
//           [this](const boost::system::error_code &ec, std::size_t bytesTransferred) {
//             if (ec)
//             {
//               auto e = std::make_shared<Error>(ec.message());
//               std::shared_ptr<Response> response(nullptr);
//               requestHandler_(e, response);
//               return;
//             }
//             receive();
//           });

//       throttlingTimer_.expires_from_now(boost::posix_time::milliseconds(150));
//     });
//     //   printf("Receive\n");
//     //   timeoutTimer_.expires_from_now(boost::posix_time::milliseconds(timeout_));
//     //   boost::asio::streambuf responseBuffer;

//     //   timeoutTimer_.async_wait([this](const boost::system::error_code &ec) {
//     //     port_.cancel();

//     //     std::shared_ptr<Response> response(nullptr);
//     //     auto e = std::make_shared<Error>("Timeout");
//     //     requestHandler_(e, response);
//     //   });

//     //   boost::asio::async_read(
//     //       port_,
//     //       responseBuffer,
//     //       boost::asio::transfer_at_least(Response::Size),
//     //       [this](const boost::system::error_code &ec, std::size_t bytesTransferred) {
//     //         std::shared_ptr<Response> response(nullptr);
//     //         if (ec)
//     //         {
//     //           auto e = std::make_shared<Error>(ec.message());
//     //           requestHandler_(e, response);
//     //           return;
//     //         }
//     //         requestHandler_(NULL, response);
//     //       });
//   }
//   else
//   {
//     std::shared_ptr<Response> response(nullptr);
//     auto e = std::make_shared<Error>("Not connected");
//     requestHandler(e, response);
//   }
// }
