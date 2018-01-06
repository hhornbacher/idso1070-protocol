#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <deque>

#include "../enums.h"
#include "../util/hexdump.h"

class Response
{
public:
  static const size_t PacketSize = 509;
  static const size_t HeaderSize = 7;
  static const size_t PayloadSize = PacketSize - HeaderSize;

private:
  struct __attribute__((packed)) RawPacket
  {
    uint8_t header[HeaderSize];
    uint8_t payload[PayloadSize];
  } rawPacket;

public:
  Response(uint8_t *data);

  uint8_t *getHeader();
  uint8_t *getPayload();
  uint8_t getCommandID();
  uint8_t getCounter();
  ResponseType getType();
  size_t getPayloadLength();

  void print();
};

#endif // _RESPONSE_H_