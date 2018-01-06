#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <deque>

#include "../enums.h"
#include "../util/hexdump.h"

#define IDSO1070A_PACKET_SIZE 509
#define IDSO1070A_PACKET_HEADER_SIZE 7
#define IDSO1070A_PACKET_PAYLOAD_SIZE 502

class Response
{
private:
  struct __attribute__((packed)) RawPacket
  {
    uint8_t header[IDSO1070A_PACKET_HEADER_SIZE];
    uint8_t payload[IDSO1070A_PACKET_PAYLOAD_SIZE];
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