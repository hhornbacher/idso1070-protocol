#include "CommandQueue.h"

void CommandQueue::add(Commands cmd)
{
    push_back(cmd);
}

void CommandQueue::addFront(Commands cmd)
{
    push_front(cmd);
}

size_t CommandQueue::getSize()
{
    return size();
}

Commands CommandQueue::getNext()
{
    Commands next = (*begin());
    pop_front();
    return next;
}