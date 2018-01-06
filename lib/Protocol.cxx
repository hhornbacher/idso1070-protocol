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

void Protocol::setSampleDataHandler(SampleDataHandler handler)
{
    sampleDataHandler = handler;
}

void Protocol::receive()
{
    connection.receive();
    while (connection.getResponseBufferSize() > 0)
    {

        if (ignoreNextResponse)
        {
            currentResponse = connection.getLatestResponse();
            delete currentResponse;
            currentResponse = NULL;
            ignoreNextResponse = false;
        }
        else
        {
            currentResponse = connection.getLatestResponse();

            if (currentCommand)
            {
                // Check for sample data packet
                if (currentResponse->getCommandID() == 0x04 && currentResponse->getType() == 0xaa)
                {
                    sampling = true;
                    if (sampleDataHandler)
                        sampleDataHandler(currentResponse);
                }
                else
                {
                    // Parse received packet
                    bool success = currentCommand->getPayload()[0] == currentResponse->getType() &&
                                   currentCommand->getPayload()[1] == currentResponse->getCommandID() &&
                                   currentCommand->callHandler(currentResponse, retries);

                    // If it's a wifi connection, then we get two responses per command
                    if (success && !connection.isUsbConnection())
                        ignoreNextResponse = true;

                    // Call handler of current command
                    if (!success && retries < MaxCommandRetries)
                    {
                        retries++;
                    }
                    else
                    {
                        // Remove current command generator
                        commandQueue.pop_front();
                        retries = 0;
                    }
                }

                // Remove current command
                delete currentCommand;
                currentCommand = NULL;
            }
            else if (sampling)
            {
                if (sampleDataHandler)
                    sampleDataHandler(currentResponse);
            }
            delete currentResponse;
            currentResponse = NULL;
        }
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