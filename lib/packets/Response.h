#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include <cstdio>

#include "../util/hexdump.h"
#include "../device/IDSO1070A.h"

#include "Packet.h"

class Response : public Packet
{
private:
  uint8_t rawPacket[PacketSize];

public:
  Response(uint8_t *data);

  uint8_t getCommandID();
  ResponseType getType();

  size_t getPayloadLength();

  void print();
};

#endif // _RESPONSE_H_