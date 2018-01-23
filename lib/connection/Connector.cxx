#include "connection/Connector.h"

#include "util/hexdump.h"

Connector::Connector() : rawBuffer(RawBufferLength)
{
}

Response *Connector::getLatestResponse()
{
  Response *response = responseBuffer.front();
  responseBuffer.pop();
  return response;
}

size_t Connector::getResponseBufferSize()
{
  return responseBuffer.size();
}

void Connector::grabPacket()
{
  while (rawBuffer.size() > Response::PacketSize)
  {
    if (rawBuffer[0] == 0xff && rawBuffer[1] == 0x01)
    {
      rawBuffer.linearize();
      Response *response = new Response(rawBuffer.array_one().first);
      rawBuffer.erase_begin(Response::PacketSize);
      responseBuffer.push(response);
    }
    else
    {
      rawBuffer.pop_front();
    }
  }
}

bool Connector::isConnected()
{
  return connected;
}