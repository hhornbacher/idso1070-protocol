#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <boost/asio.hpp>

class Packet
{
public:
  typedef std::vector<uint8_t> DataVector;

  void setData(boost::asio::streambuf &data);
  DataVector &getData();
  boost::asio::mutable_buffers_1 getBuffer();
  size_t getSize() const;
  std::string toString();

protected:
  DataVector data_;
};