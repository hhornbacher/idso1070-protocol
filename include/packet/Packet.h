#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <boost/asio.hpp>

class Packet
{
public:
  typedef std::vector<uint8_t> DataVector;

  void clearData();

  DataVector &getData();
  std::string toString() const;

protected:
  DataVector data_;
};