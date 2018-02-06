#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "base.h"
#include "IDSO1070.h"
#include "Packet.h"
#include "Response.h"

class Sample : public Packet
{
public:
  Sample(uint8_t *data);
  Sample(Response *response);

  size_t getPayloadLength();

private:
  uint8_t rawPacket[PacketSize];
};

#endif // _SAMPLE_H_