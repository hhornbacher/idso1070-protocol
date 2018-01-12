#ifndef FIFO_SERIALIZER_H
#define FIFO_SERIALIZER_H

#include "Serializer.h"

class FIFOSerializer : Serializer
{
  private:
    bool enabled;

  public:
    FIFOSerializer(const char *path, bool enabled);

    void serialize(const void *data, size_t length);
};

#endif