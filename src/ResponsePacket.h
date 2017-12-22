#ifndef _RESPONSE_PACKET_H_
#define _RESPONSE_PACKET_H_

#include <cstdint>

#include <deque>

#define IDSO10790A_PACKET_SIZE 509

struct __attribute__((packed)) Packet
{
    uint8_t header[7];
    uint8_t payload[IDSO10790A_PACKET_SIZE - 7];
};

typedef std::deque<Packet *> PacketQueue;

#endif // _RESPONSE_PACKET_H_