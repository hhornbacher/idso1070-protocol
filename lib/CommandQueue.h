#ifndef _COMMAND_QUEUE_H_
#define _COMMAND_QUEUE_H_

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <deque>

#include "enums.h"

class CommandQueue : private std::deque<Commands>
{
  public:
    void addFront(Commands cmd);
    void add(Commands cmd);

    size_t getSize();
    Commands getNext();
};

#endif // _COMMAND_QUEUE_H_