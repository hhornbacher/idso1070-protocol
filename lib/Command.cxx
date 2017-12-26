#include "Command.h"

Command::Command(CommandCode cmd)
{
    payload[0] = 0x55;
    payload[1] = (uint8_t)cmd;
    payload[2] = 0;
    payload[3] = 0;
}

Command::Command(uint8_t *cmd)
{
    memcpy(payload, cmd, 4);
}

Command::Command(uint8_t *cmd, uint8_t responseCount) : responseCount(responseCount)
{
    memcpy(payload, cmd, 4);
}

uint8_t *Command::getPayload()
{
    return payload;
}
uint8_t Command::getResponseCount()
{
    return responseCount;
}

void CommandQueue::add(Command *cmd)
{
    if (cmd != NULL)
    {
        totalResponseCount += cmd->getResponseCount();
        push_back(cmd);
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