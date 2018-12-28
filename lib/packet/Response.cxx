#include <packet/Response.h>

#include <sstream>

using namespace std;

Response::Response()
{
  data_.reserve(Size);
}

void Response::getPayload(DataVector &data) const
{
  data.assign(data_.begin() + 7, data_.end());
}

bool Response::isValid() const
{
  return data_[0] == 0xff && data_[1] == 0x01 && data_[508] == 0xce;
}

string Response::toString() const
{
  stringstream ss;
  ss << "Response:\n";

  if (isValid())
  {
    ss << "Valid: true\n";
  }
  else
  {
    ss << "Valid: false\n";
  }

  ss << Packet::toString();
  return ss.str();
}