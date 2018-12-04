#pragma once

class Exception : public std::runtime_error
{
public:
  Exception(std::string reason)
      : runtime_error("protocol error"), reason_(reason)
  {
  }

  virtual const char *what() const throw()
  {
    return reason_.c_str();
  }

private:
  std::string reason_;
};