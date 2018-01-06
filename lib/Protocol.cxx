#include "Protocol.h"

Protocol::Protocol(Connector &connection) : connection(connection),
                                            commandTimeout(200)
{
    // device.getChannel1().parseChVoltsDivStatus = PARSE_CHVOLTSDIV_S0;
    // device.getChannel2().parseChVoltsDivStatus = PARSE_CHVOLTSDIV_S0;
    // this.autoMeasure.setAutoMeasureChannel(AutoMeasureChannel.CH1);
    // this.autoMeasure.setDvmChannel(DVMChannel.CH1);
}

Protocol::~Protocol()
{
}

void Protocol::start()
{
    connection.start();
}

void Protocol::stop()
{
    connection.stop();
}

IDSO1070A &Protocol::getDevice()
{
    return device;
}

void Protocol::sendCommand(CommandGenerator cmdFn)
{
    commandQueue.push_back(cmdFn);
}

void Protocol::sendCommands(CommandGeneratorVector cmdFns)
{
    for (auto cmdFn : cmdFns)
        commandQueue.push_back(cmdFn);
}

void Protocol::receive()
{
    size_t len = connection.receive();
    if (len == IDSO1070A_PACKET_SIZE)
    {

        if (ignoreNextResponse)
        {
            ignoreNextResponse = false;
        }
        else
        {
            currentResponse = new Response(connection.getPacketBuffer());

            // Check if we have a sample data packet
            if (currentResponse->getType() == 0xaa &&
                currentResponse->getCommandID() == 0x04)
            {
                printf("Got sample data!\n");
                if (currentCommand &&
                    currentCommand->getPayload()[0] == 0xaa &&
                    currentCommand->getPayload()[1] == 0x04)
                {
                    commandQueue.pop_front();
                    delete currentCommand;
                }
            }
            else
            {
                // Parse received packet
                bool success = currentCommand->getPayload()[0] == currentResponse->getType() &&
                               currentCommand->getPayload()[1] == currentResponse->getCommandID() &&
                               currentCommand->callHandler(currentResponse, retries);

                // If command was readEEROM and it's a wifi connection, then we get two responses per command
                if (currentCommand->getPayload()[0] == TYPE_EE && !connection.isUsbConnection())
                    ignoreNextResponse = true;

                // Call handler of current command
                if (!success && retries < COMMAND_MAX_RETRIES)
                    retries++;
                else
                {
                    // Remove current command generator
                    commandQueue.pop_front();
                    retries = 0;
                }

                // Remove current command
                delete currentCommand;
            }
        }
        connection.clearPacketBuffer();
    }
}

void Protocol::transmit()
{
    if (commandTimeout.isTimedOut() && commandQueue.size() > 0)
    {
        CommandGenerator generateCommand = *(commandQueue.begin());
        currentCommand = generateCommand();

        // Transmit current command
        connection.transmit(currentCommand->getPayload(), 4);

        commandTimeout.reset();
    }
}

void Protocol::process()
{
    transmit();
    receive();
}

void Protocol::print()
{
    device.print();
}