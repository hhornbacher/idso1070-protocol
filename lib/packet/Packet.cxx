#include <packet/Packet.h>

#include <util.h>
#include <sstream>

using namespace boost::asio;
using namespace std;

void Packet::clearData()
{
  data_.clear();
}

void Packet::setData(const DataVector &data)
{
  data_.assign(data.begin(), data.end());
}

const Packet::DataVector &Packet::getData()
{
  return data_;
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