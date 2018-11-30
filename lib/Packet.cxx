#include <Packet.h>

using namespace boost::asio;

void Packet::setData(boost::asio::streambuf &data)
{
  const uint8_t *dataArray = boost::asio::buffer_cast<const uint8_t *>(data.data());
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