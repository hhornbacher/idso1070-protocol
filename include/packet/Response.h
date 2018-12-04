#pragma once

#include "Packet.h"

class Response : public Packet
{
public:
  static constexpr size_t Size = 509;

  void setErrorMessage(std::string message);
  std::string getErrorMessage() const;

  const uint8_t *getPayload() const;
  bool isValid() const;
  std::string toString() const;

private:
  std::string errorMessage_;
  bool failed_{false};
};