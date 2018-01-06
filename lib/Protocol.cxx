#include "Protocol.h"

Protocol::Protocol(Connector &connection) : connection(connection),
                                            commandTimeout(200),
                                            cmdFactory(device)
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

void Protocol::init()
{
    sendCommand(cmdFactory.selectRAMChannel());
    sendCommand(cmdFactory.readARMVersion());
    sendCommand(cmdFactory.readFPGAVersion());
    sendCommands(cmdFactory.readEEROMPages());
    sendCommand(cmdFactory.updateSampleRate());
    sendCommand(cmdFactory.getFreqDivLowBytes());
    sendCommand(cmdFactory.getFreqDivHighBytes());
    sendCommand(cmdFactory.selectChannel());
    sendCommand(cmdFactory.updateTriggerSourceAndSlope());
    sendCommand(cmdFactory.updateTriggerLevel());
    sendCommand(cmdFactory.preTrigger());
    sendCommand(cmdFactory.postTrigger());
    sendCommand(cmdFactory.readRamCount());
    sendCommand(cmdFactory.selectRAMChannel());
    sendCommand(cmdFactory.updateChannelVolts125());
    sendCommand(cmdFactory.relay1());
    sendCommand(cmdFactory.relay2());
    sendCommand(cmdFactory.relay3());
    sendCommand(cmdFactory.relay4());
    sendCommand(cmdFactory.channel1Level());
    sendCommand(cmdFactory.channel2Level());
    sendCommand(cmdFactory.updateChannelVolts125());
    sendCommand(cmdFactory.updateTriggerMode());
    sendCommand(cmdFactory.updateTriggerLevel());
    sendCommand(cmdFactory.channel1Coupling());
    sendCommand(cmdFactory.channel2Coupling());
}

void Protocol::setSamplePacketHandler(SamplePacketHandler handler)
{
    samplePacketHandler = handler;
}

void Protocol::setResponsePacketHandler(ResponsePacketHandler handler)
{
    responsePacketHandler = handler;
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
                    if (samplePacketHandler)
                        samplePacketHandler(currentResponse);
                }
                else
                {
                    // Parse received packet
                    bool success = currentCommand->getPayload()[0] == currentResponse->getType() &&
                                   currentCommand->getPayload()[1] == currentResponse->getCommandID() &&
                                   responsePacketHandler && responsePacketHandler(currentCommand, currentResponse, retries);

                    // If it's a wifi connection, then we get two responses per command
                    if (success && !connection.isUsbConnection())
                        ignoreNextResponse = true;

                    // Call handler of current command
                    if (!success && retries < MaxCommandRetries)
                        retries++;
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
                if (samplePacketHandler)
                    samplePacketHandler(currentResponse);
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