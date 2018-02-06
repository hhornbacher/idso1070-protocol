#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "base.h"
#include "IDSO1070.h"
#include "Packet.h"
#include "Response.h"

#include <boost/circular_buffer.hpp>

class Sample : public Packet
{
public:
  static const int SampleBufferSize = 1024 * 16;

  struct SampleBuffer
  {
    boost::circular_buffer<int8_t> channel1;
    boost::circular_buffer<int8_t> channel2;

    SampleBuffer() : channel1(SampleBufferSize), channel2(SampleBufferSize) {}
  };

  Sample(uint8_t *data);
  Sample(Response *response);

  size_t getPayloadLength();

private:
  uint8_t rawPacket[PacketSize];
};

#endif // _SAMPLE_H_