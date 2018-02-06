#ifndef _SAMPLE_BUFFER_H_
#define _SAMPLE_BUFFER_H_

#include "base.h"

#include <boost/circular_buffer.hpp>

struct SampleBuffer
{
    static const int SampleBufferSize = 1024 * 16;
    boost::circular_buffer<int8_t> channel1;
    boost::circular_buffer<int8_t> channel2;

    SampleBuffer() : channel1(SampleBufferSize), channel2(SampleBufferSize) {}
};

#endif // _SAMPLE_BUFFER_H_