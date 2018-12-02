#pragma once

#include "Packet.h"

class Response : public Packet
{
public:
  static constexpr size_t Size = 509;

  const uint8_t *getPayload() const;
  bool isValid() const;
  std::string toString() const;
};