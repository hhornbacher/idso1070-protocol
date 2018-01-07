#include "Connector.h"

#include "../util/hexdump.h"

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
  while (rawBufferLength > Response::PacketSize)
  {
    uint8_t packetSignature[2] = {0xff, 0x01};
    for (size_t pos = 0; pos < rawBufferLength; pos++)
    {
      if (memcmp(&rawBuffer[pos], packetSignature, 2) == 0)
      {
        if ((rawBufferLength - pos) > Response::PacketSize)
        {
          Response *response = new Response(rawBuffer);
          rawBufferLength -= Response::PacketSize + pos;
          memcpy(rawBuffer, &rawBuffer[Response::PacketSize + pos], rawBufferLength);
          responseBuffer.push(response);
          break;
        }
        else
        {
          rawBufferLength -= pos;
          memcpy(rawBuffer, &rawBuffer[pos], rawBufferLength);
          break;
        }
      }
    }
  }
}

bool Connector::isUsbConnection()
{
  return usbConnection;
}