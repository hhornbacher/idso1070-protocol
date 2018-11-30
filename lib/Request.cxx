#include <Request.h>

Request::Request(Type type, Command command, uint16_t params)
{
  data_.push_back((uint8_t)type);
  data_.push_back((uint8_t)command);
  data_.push_back((uint8_t)(params & 0xff));
  data_.push_back((uint8_t)((params >> 8) & 0xff));
}

Request::Request(Type type, Command command, uint8_t param1, uint8_t param2)
{
  data_.push_back((uint8_t)type);
  data_.push_back((uint8_t)command);
  data_.push_back(param1);
  data_.push_back(param2);
}