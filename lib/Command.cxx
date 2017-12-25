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
