#pragma once

#include "Packet.h"

class Response : public Packet
{
public:
  const uint8_t *getPayload() const;
  bool isValid() const;

  std::string toString() const;
};