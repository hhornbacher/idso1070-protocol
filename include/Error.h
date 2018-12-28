#pragma once

#include <memory>

class Error
{
public:
  Error(std::string reason)
      : reason_(reason)
  {
  }

  const char *what()
  {
    return reason_.c_str();
  }

private:
  std::string reason_;
};