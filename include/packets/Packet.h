#ifndef _PACKET_H_
#define _PACKET_H_

#include "base.h"

class Packet
{
public:
  static const size_t PacketSize = 509;
  static const size_t HeaderSize = 7;
  static const size_t PayloadSize = PacketSize - HeaderSize;

  Packet(uint8_t *data);

  uint8_t getCounter();

  uint8_t *getHeader();
  uint8_t *getPayload();
  virtual size_t getPayloadLength() = 0;

private:
  uint8_t rawPacket[PacketSize];
};

#endif // _PACKET_H_