#include <Response.h>

#include <sstream>

using namespace std;

const uint8_t *Response::getPayload() const
{
  return &data_[7];
}

bool Response::isValid() const
{
  return data_[0] == 0xff && data_[1] == 0x01 && data_[508] == 0xce;
}

string Response::toString() const
{
  stringstream ss;
  ss << "Response:\n";

  ss << Packet::toString();
  return ss.str();
}