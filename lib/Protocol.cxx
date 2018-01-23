#include "Protocol.h"

Protocol::Protocol() : commandTimeout(200),
                       packetParser(device)
{
}

Protocol::~Protocol()
{
}

void Protocol::connect(string serialDevice)
{
    if (!connector)
    {
        connector = new USBConnector(serialDevice);

        try
        {
            connector->start();
        }
        catch (ConnectException &e)
        {
            connectError = e.what();
        }
    }
}

void Protocol::connect(string serverHost, int port)
{
    if (!connector)
    {
        connector = new TCPConnector(serverHost, port);

        try
        {
            connector->start();
        }
        catch (ConnectException &e)
        {
            connectError = e.what();
        }
    }
}

void Protocol::disconnect()
{
    if (connector)
    {
        connector->stop();
        delete connector;
        connector = NULL;
    }
}

IDSO1070 &Protocol::getDevice()
{
    return device;
}

void Protocol::sendCommand(CommandGenerator cmdFn)
{
    commandQueue.push_back(cmdFn);
    commandCount++;
}

void Protocol::init()
{
    sendCommand(cmdFactory.updateRAMChannelSelection(device.getChannel1().isEnabled(), device.getChannel2().isEnabled()));
    sendCommand(cmdFactory.readARMVersion());
    sendCommand(cmdFactory.readFPGAVersion());
    sendCommand(cmdFactory.readEEROMPage(0x00));
    sendCommand(cmdFactory.readEEROMPage(0x04));
    sendCommand(cmdFactory.readEEROMPage(0x05));
    sendCommand(cmdFactory.readEEROMPage(0x07));
    sendCommand(cmdFactory.readEEROMPage(0x08));
    sendCommand(cmdFactory.readEEROMPage(0x09));
    sendCommand(cmdFactory.readEEROMPage(0x0a));
    sendCommand(cmdFactory.readEEROMPage(0x0b));
    sendCommand(cmdFactory.readEEROMPage(0x0c));
    sendCommand(cmdFactory.updateSampleRate(device.getTimeBase(), device.getEnabledChannelsCount()));
    sendCommand(cmdFactory.updateFreqDivLowBytes(device.getTimebaseFromFreqDiv()));
    sendCommand(cmdFactory.updateFreqDivHighBytes(device.getTimebaseFromFreqDiv()));
    sendCommand(cmdFactory.updateChannelSelection(device.getChannel1().isEnabled(), device.getChannel2().isEnabled(), device.isSampleRate200Mor250M()));
    sendCommand(cmdFactory.updateTriggerSourceAndSlope(device.getTrigger().getChannel(), device.getScopeMode(), device.getTrigger().getSlope()));
    sendCommand(cmdFactory.updateTriggerLevel(device.getTrigger().getLevel(), device.getTrigger().getTopPWM(), device.getTrigger().getBottomPWM()));
    sendCommand(cmdFactory.updatePreTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTrigger().getXPosition()));
    sendCommand(cmdFactory.updatePostTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTrigger().getXPosition()));
    sendCommand(cmdFactory.readRamCount(device.getEnabledChannelsCount(), device.getSamplesNumberOfOneFrame(), device.isSampleRate200Mor250M(), device.getTrigger().getXPosition(), device.getPacketsNumber()));
    sendCommand(cmdFactory.updateRAMChannelSelection(device.getChannel1().isEnabled(), device.getChannel2().isEnabled()));

    sendCommand(cmdFactory.updateChannelVolts125(device.getChannel1().getVerticalDiv(), device.getChannel2().getVerticalDiv()));
    sendCommand(cmdFactory.updateRelay1(device.getChannel1().getVerticalDiv()));
    sendCommand(cmdFactory.updateRelay2(device.getChannel1().getVerticalDiv()));
    sendCommand(cmdFactory.updateRelay3(device.getChannel2().getVerticalDiv()));
    sendCommand(cmdFactory.updateRelay4(device.getChannel2().getVerticalDiv()));

    sendCommand(cmdFactory.updateChannel1Level(device.getChannel1().getVerticalDiv(), device.getChannel1().getVerticalPosition(), device.getChannel1().getPWM((uint8_t)device.getChannel1().getVerticalDiv(), 0), device.getChannel1().getPWM((uint8_t)device.getChannel1().getVerticalDiv(), 1)));
    sendCommand(cmdFactory.updateChannel2Level(device.getChannel2().getVerticalDiv(), device.getChannel2().getVerticalPosition(), device.getChannel2().getPWM((uint8_t)device.getChannel2().getVerticalDiv(), 0), device.getChannel2().getPWM((uint8_t)device.getChannel2().getVerticalDiv(), 1)));
    sendCommand(cmdFactory.updateChannelVolts125(device.getChannel1().getVerticalDiv(), device.getChannel2().getVerticalDiv()));
    sendCommand(cmdFactory.updateTriggerMode(device.getCaptureMode(), device.getTrigger().getMode(), device.getScopeMode()));
    sendCommand(cmdFactory.updateTriggerLevel(device.getTrigger().getLevel(), device.getTrigger().getTopPWM(), device.getTrigger().getBottomPWM()));

    sendCommand(cmdFactory.updateChannel1Coupling(device.getChannel1().getCoupling()));
    sendCommand(cmdFactory.updateChannel2Coupling(device.getChannel2().getCoupling()));
}

void Protocol::startSampling()
{
    sendCommand(cmdFactory.startSampling());
}

void Protocol::setProgressHandler(ProgressHandler handler)
{
    progressHandler = handler;
}

void Protocol::receive()
{
    connector->receive();
    while (connector->getResponseBufferSize() > 0)
    {

        // This is only for some packets in wifi mode, drops current packet
        if (ignoreNextResponse)
        {
            currentResponse = connector->getLatestResponse();
            delete currentResponse;
            currentResponse = NULL;
            ignoreNextResponse = false;
        }
        else
        {
            currentResponse = connector->getLatestResponse();

            if (currentCommand)
            {
                // Check for sample data packet
                if (currentResponse->getCommandCode() == 0x04 && currentResponse->getCommandType() == TYPE_CONTROL)
                {
                    // Enable sampling mode
                    sampling = true;

                    // Create and pare sample
                    Sample *sample = new Sample(currentResponse);
                    packetParser.parse(sample);

                    // Remove sample packet
                    delete sample;

                    // Remove current command generator
                    commandQueue.pop_front();
                    retries = 0;

                    // Remove current command
                    delete currentCommand;
                    currentCommand = NULL;
                }
                else
                {
                    // Check & parse received packet
                    bool success = currentCommand->getPayload()[0] == currentResponse->getCommandType() &&
                                   currentCommand->getPayload()[1] == currentResponse->getCommandCode() &&
                                   packetParser.parse(currentResponse);

                    // If it's a wifi connector, then we get two responses per command
                    if (success && connector->getType() == CONNECTOR_WIFI)
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
                    }
                }

                // Remove current command
                delete currentCommand;
                currentCommand = NULL;
            }
            else if (sampling)
            {
                Sample *sample = new Sample(currentResponse->getHeader());
                packetParser.parse(sample);
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
        connector->transmit(currentCommand->getPayload(), 4);

        // Reset command limiting timout
        commandTimeout.reset();
    }
    else if (commandQueue.size() == 0 && commandCount != 0)
    {
        commandCount = 0;
        progressHandler(1.0f);
    }
}

void Protocol::process()
{
    if (connector && connector->isConnected())
    {
        transmit();
        receive();
    }
}

bool Protocol::isSampling()
{
    return sampling;
}

string Protocol::getConnectError()
{
    return connectError;
}

Connector *Protocol::getConnector()
{
    return connector;
}