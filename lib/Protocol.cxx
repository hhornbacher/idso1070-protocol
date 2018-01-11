#include "Protocol.h"

Protocol::Protocol(Connector &connection) : connection(connection),
                                            commandTimeout(200),
                                            cmdFactory(device),
                                            responseParser(device),
                                            sampleParser(device)
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
    commandCount++;
}

void Protocol::sendCommands(CommandGeneratorVector cmdFns)
{
    commandCount += cmdFns.size();
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

void Protocol::setProgressHandler(ProgressHandler handler)
{
    progressHandler = handler;
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
                    Sample *sample = new Sample(currentResponse);
                    sample->print();
                    sampleParser.parse(sample);
                    delete sample;
                }
                else
                {
                    // Check & parse received packet
                    bool success = currentCommand->getPayload()[0] == currentResponse->getType() &&
                                   currentCommand->getPayload()[1] == currentResponse->getCommandID() &&
                                   responseParser.parse(currentResponse);

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

                        // Update progress
                        progressHandler(((float)commandCount - (float)commandQueue.size()) / (float)commandCount);
                        if (commandQueue.size() == 0)
                            commandCount = 0;
                    }
                }

                // Remove current command
                delete currentCommand;
                currentCommand = NULL;
            }
            else if (sampling)
            {
                Sample *sample = new Sample(currentResponse->getHeader());

                // sampleParser.parse(sample);
                delete sample;
            }
            delete currentResponse;
            currentResponse = NULL;
        }
    }
}

void Protocol::transmit()
{
    // Check if there are any commands and command limiting timeout
    if (commandQueue.size() > 0 && commandTimeout.isTimedOut())
    {
        // Create Command with CommandGenerator function
        CommandGenerator generateCommand = *(commandQueue.begin());
        currentCommand = generateCommand();

        // Transmit current command
        connection.transmit(currentCommand->getPayload(), 4);

        // Reset command limiting timout
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