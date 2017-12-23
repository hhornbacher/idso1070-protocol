#ifndef _RESPONSE_PACKET_H_
#define _RESPONSE_PACKET_H_

#include <cstdint>
#include <cstring>

#include <deque>

#define IDSO10790A_PACKET_SIZE 509

class ResponsePacket
{
  private:
    struct __attribute__((packed)) Packet
    {
        uint8_t header[7];
        uint8_t payload[IDSO10790A_PACKET_SIZE - 7];
    } rawPacket;

  public:
    ResponsePacket(uint8_t *data);

    uint8_t *getHeader();
    uint8_t *getPayload();
    size_t getPayloadLength();
};

typedef std::deque<ResponsePacket *> PacketQueue;

#endif // _RESPONSE_PACKET_H_