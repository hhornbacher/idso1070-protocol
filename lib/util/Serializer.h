#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

class Serializer
{
protected:
  int fileHandle;

public:
  Serializer(const char *path);

  void serialize(const void *data, size_t length);
};

#endif