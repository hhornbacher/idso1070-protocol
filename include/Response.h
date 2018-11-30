#pragma once

#include "Packet.h"

class Response : public Packet
{
public:
  uint8_t getCounter() const;
  const uint8_t *getPayload() const;
};