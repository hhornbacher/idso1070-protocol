#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <deque>

#include "../util/hexdump.h"

#define IDSO1070A_PACKET_SIZE 509
#define IDSO1070A_PACKET_HEADER_SIZE 7
#define IDSO1070A_PACKET_PAYLOAD_SIZE 502

enum ResponseType
{
  TYPE_AA = 0xaa,
  TYPE_EE = 0xee,
  TYPE_FPGA = 0x55,
  TYPE_STATE = 0x57
};

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
  ResponseType getType();
  size_t getPayloadLength();

  void print();
  static const char *typeToString(ResponseType type);
};

#endif // _RESPONSE_H_