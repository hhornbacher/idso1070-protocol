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
  if (rawBufferLength > Response::PacketSize)
  {
    Response *response = new Response(rawBuffer);
    // printf("old buffer length: %ld\n", rawBufferLength);
    // hexdump(rawBuffer, rawBufferLength);
    rawBufferLength -= Response::PacketSize;
    // printf("new buffer length: %ld\n", rawBufferLength);
    // hexdump(rawBuffer, rawBufferLength);
    memcpy(rawBuffer, &rawBuffer[Response::PacketSize], rawBufferLength);
    // hexdump(rawBuffer, rawBufferLength);
    responseBuffer.push(response);
  }
}

bool Connector::isUsbConnection()
{
  return usbConnection;
}