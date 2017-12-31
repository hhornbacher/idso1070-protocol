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

void Command::setName(const char *name)
{
    strncpy(this->name, name, 256);
}

void Command::print()
{
    printf("[Command:%s]\n", name);
    printf("{%02x, %02x, %02x, %02x}\n", payload[0], payload[1], payload[2], payload[3]);
    printf("commandID = %02x\n", payload[1]);
    printf("args = %02x %02x\n", payload[2], payload[3]);
    printf("\n\n");
}

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