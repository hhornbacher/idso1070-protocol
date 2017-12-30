#include "Command.h"

Command::Command(CommandCode cmd, uint8_t param1, uint8_t param2)
{
    name[0] = 0;
    payload[0] = 0x55;
    payload[1] = (uint8_t)cmd;
    payload[2] = param1;
    payload[3] = param2;
}

Command::Command(uint8_t *payload)
{
    name[0] = 0;
    memcpy(this->payload, payload, 4);
}

uint8_t *Command::getPayload()
{
    return payload;
}
uint8_t Command::getResponseCount()
{
    return responseCount;
}
uint8_t Command::setResponseCount(uint8_t responseCount)
{
    this->responseCount = responseCount;
}

void Command::setName(const char *name)
{
    strncpy(this->name, name, 256);
}

void Command::print()
{
    printf("[Command:%s]\n", name);
    printf("commandID = %02x\n", payload[1]);
    printf("args = %02x %02x\n", payload[2], payload[3]);
    printf("responseCount = %d\n", responseCount);
    printf("\n\n");
}

void CommandQueue::add(Command *cmd)
{
    if (cmd != NULL)
    {
        totalResponseCount += cmd->getResponseCount();
        push_back(cmd);
    }
}

void CommandQueue::addFront(Command *cmd)
{
    if (cmd != NULL)
    {
        totalResponseCount += cmd->getResponseCount();
        push_front(cmd);
    }
}

void CommandQueue::add(CommandQueue cmds)
{
    for (CommandQueue::iterator i = cmds.begin(); i != cmds.end(); i++)
    {
        if ((*i) != NULL)
        {
            totalResponseCount += (*i)->getResponseCount();
            push_back(*i);
        }
    }
}

size_t CommandQueue::getTotalResponseCount()
{
    return totalResponseCount;
}

size_t CommandQueue::getSize()
{
    return size();
}

Command *CommandQueue::getNext()
{
    Command *next = (*begin());
    pop_front();
    return next;
}