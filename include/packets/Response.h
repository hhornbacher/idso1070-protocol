#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "base.h"
#include "IDSO1070.h"
#include "Packet.h"

class Response : public Packet
{
public:
  Response(uint8_t *data);

  uint8_t getCommandCode();
  CommandType getCommandType();

  size_t getPayloadLength();

private:
  uint8_t rawPacket[PacketSize];
};

#endif // _RESPONSE_H_