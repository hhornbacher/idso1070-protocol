#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <cstdio>

#include "../util/hexdump.h"
#include "../device/IDSO1070A.h"

#include "Packet.h"
#include "Response.h"

class Sample : public Packet
{
private:
  uint8_t rawPacket[PacketSize];

public:
  Sample(uint8_t *data);
  Sample(Response *response);

  uint8_t *getPayload();
  size_t getPayloadLength();

  void print();
};

#endif // _SAMPLE_H_