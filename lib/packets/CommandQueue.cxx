#include "CommandQueue.h"

void CommandQueue::add(Command *cmd)
{
    push_back(cmd);
}

size_t CommandQueue::getSize()
{
    return size();
}

Command &CommandQueue::getCurrent()
{
    return *(*begin());
}

void CommandQueue::destroyCurrent()
{
    delete (*begin());
    pop_front();
}