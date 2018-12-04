#include <packet/Packet.h>

#include <util.h>
#include <sstream>

using namespace boost::asio;
using namespace std;

void Packet::clearData()
{
  data_.clear();
}

void Packet::setData(boost::asio::streambuf &data)
{
  const uint8_t *dataArray = buffer_cast<const uint8_t *>(data.data());
  data_.insert(data_.begin(), dataArray, &dataArray[data.size()]);
}

Packet::DataVector &Packet::getData()
{
  return data_;
}

mutable_buffers_1 Packet::getBuffer()
{
  return buffer(data_);
}

size_t Packet::getSize() const
{
  return data_.size();
}

string Packet::toString() const
{
  stringstream ss;
  ss << "Packet length: " << data_.size() << " bytes\n";
  ss << "Hexdump:\n";
  ss << hexdump(data_.data(), data_.size(), 8);
  return ss.str();
}