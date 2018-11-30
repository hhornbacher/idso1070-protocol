#include <Response.h>

// uint8_t Response::getCounter() const
// {
//   return data_[2];
// }

const uint8_t *Response::getPayload() const
{
  return &data_[7];
}

bool Response::isValid() const
{
  return data_[0] == 0xff && data_[1] == 0x01 && data_[508] == 0xce;
}
