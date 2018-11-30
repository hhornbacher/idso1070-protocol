#include <Response.h>

uint8_t Response::getCounter() const
{
  return data_[2];
}

const uint8_t *Response::getPayload() const
{
  return &data_[7];
}