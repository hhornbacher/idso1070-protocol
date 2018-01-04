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

void Command::print()
{
    printf("[Command:%s]\n", name);
    printf("{%02x, %02x, %02x, %02x}\n", payload[0], payload[1], payload[2], payload[3]);
    printf("commandID = %02x\n", payload[1]);
    printf("args = %02x %02x\n", payload[2], payload[3]);
    printf("\n\n");
}

uint8_t *Command::getPayload()
{
    return payload;
}

void Command::setName(const char *name)
{
    strncpy(this->name, name, 256);
}

void Command::setHandler(Handler &handler)
{
    this->handler = &handler;
}

bool Command::callHandler(uint8_t *responsePayload, bool success)
{
    if (handler)
        return (*handler)(payload, responsePayload, success);
    return true;
}

bool Command::isInProgress()
{
    return inProgress;
}

void Command::beginProgress()
{
    inProgress = true;
}