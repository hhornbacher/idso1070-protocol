#include "packets/Command.h"

Command::Command(CommandCode cmd, uint8_t param1, uint8_t param2)
{
    payload[0] = 0x55;
    payload[1] = (uint8_t)cmd;
    payload[2] = param1;
    payload[3] = param2;
}

Command::Command(uint8_t *payload)
{
    memcpy(this->payload, payload, 4);
}

uint8_t *Command::getPayload()
{
    return payload;
}

void Command::callResponseHandler()
{
    if (responseHandler)
        responseHandler();
}

void Command::setResponseHandler(ResponseHandler responseHandler)
{
    this->responseHandler = responseHandler;
}