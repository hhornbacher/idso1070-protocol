#pragma once

#include "Packet.h"

#include <memory>

class Response : public Packet
{
public:
  static constexpr size_t Size = 509;

  void getPayload(DataVector &buffer) const;
  bool isValid() const;
  std::string toString() const;
};