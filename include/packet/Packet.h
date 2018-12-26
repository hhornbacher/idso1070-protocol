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
  void setData(const DataVector &data);

  const DataVector &getData();
  size_t getSize() const;
  std::string toString() const;

protected:
  DataVector data_;
};