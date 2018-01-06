#ifndef _PACKET_H_
#define _PACKET_H_

#include <cstdint>
#include <cstring>

class Packet
{
public:
  static const size_t PacketSize = 509;
  static const size_t HeaderSize = 7;
  static const size_t PayloadSize = PacketSize - HeaderSize;

private:
  uint8_t rawPacket[PacketSize];

public:
  Packet(uint8_t *data);

  uint8_t getCounter();

  uint8_t *getHeader();

  virtual uint8_t *getPayload() = 0;
  virtual size_t getPayloadLength() = 0;

  virtual void print() = 0;
};

#endif // _PACKET_H_