#include "Protocol.h"

Protocol::Protocol() : commandTimeout(200),
                       packetParser(device)
{
    progressHandler = [](float) {};
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

void Protocol::sendCommand(Command *cmd)
{
    commandQueue.push_back(cmd);
}

void Protocol::init()
{
    deque<Command *> initCommandList;
    initCommandList.push_back(cmdFactory.updateRAMChannelSelection(device.getChannel1().isEnabled(), device.getChannel2().isEnabled()));
    initCommandList.push_back(cmdFactory.readARMVersion());
    initCommandList.push_back(cmdFactory.readFPGAVersion());
    initCommandList.push_back(cmdFactory.readEEROMPage(0x00));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x04));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x05));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x07));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x08));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x09));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x0a));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x0b));
    initCommandList.push_back(cmdFactory.readEEROMPage(0x0c));
    initCommandList.push_back(cmdFactory.updateSampleRate(device.getTimeBase(), device.getEnabledChannelsCount()));
    initCommandList.push_back(cmdFactory.updateFreqDivLowBytes(device.getTimebaseFromFreqDiv()));
    initCommandList.push_back(cmdFactory.updateFreqDivHighBytes(device.getTimebaseFromFreqDiv()));
    initCommandList.push_back(cmdFactory.updateChannelSelection(device.getChannel1().isEnabled(), device.getChannel2().isEnabled(), device.isSampleRate200Mor250M()));
    initCommandList.push_back(cmdFactory.updateTriggerSourceAndSlope(device.getTrigger().getChannel(), device.getScopeMode(), device.getTrigger().getSlope()));
    initCommandList.push_back(cmdFactory.updateTriggerLevel(device.getTrigger().getLevel(), device.getTrigger().getTopPWM(), device.getTrigger().getBottomPWM()));
    initCommandList.push_back(cmdFactory.updatePreTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTrigger().getXPosition()));
    initCommandList.push_back(cmdFactory.updatePostTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTrigger().getXPosition()));
    initCommandList.push_back(cmdFactory.readRamCount(device.getEnabledChannelsCount(), device.getSamplesNumberOfOneFrame(), device.isSampleRate200Mor250M(), device.getTrigger().getXPosition(), device.getPacketsNumber()));
    initCommandList.push_back(cmdFactory.updateRAMChannelSelection(device.getChannel1().isEnabled(), device.getChannel2().isEnabled()));
    initCommandList.push_back(cmdFactory.updateChannelVolts125(device.getChannel1().getVerticalDiv(), device.getChannel2().getVerticalDiv()));
    initCommandList.push_back(cmdFactory.updateRelay1(device.getChannel1().getVerticalDiv()));
    initCommandList.push_back(cmdFactory.updateRelay2(device.getChannel1().getVerticalDiv()));
    initCommandList.push_back(cmdFactory.updateRelay3(device.getChannel2().getVerticalDiv()));
    initCommandList.push_back(cmdFactory.updateRelay4(device.getChannel2().getVerticalDiv()));
    initCommandList.push_back(cmdFactory.updateChannel1Level(device.getChannel1().getVerticalDiv(), device.getChannel1().getVerticalPosition(), device.getChannel1().getPWM((uint8_t)device.getChannel1().getVerticalDiv(), 0), device.getChannel1().getPWM((uint8_t)device.getChannel1().getVerticalDiv(), 1)));
    initCommandList.push_back(cmdFactory.updateChannel2Level(device.getChannel2().getVerticalDiv(), device.getChannel2().getVerticalPosition(), device.getChannel2().getPWM((uint8_t)device.getChannel2().getVerticalDiv(), 0), device.getChannel2().getPWM((uint8_t)device.getChannel2().getVerticalDiv(), 1)));
    initCommandList.push_back(cmdFactory.updateChannelVolts125(device.getChannel1().getVerticalDiv(), device.getChannel2().getVerticalDiv()));
    initCommandList.push_back(cmdFactory.updateTriggerMode(device.getCaptureMode(), device.getTrigger().getMode(), device.getScopeMode()));
    initCommandList.push_back(cmdFactory.updateTriggerLevel(device.getTrigger().getLevel(), device.getTrigger().getTopPWM(), device.getTrigger().getBottomPWM()));
    initCommandList.push_back(cmdFactory.updateChannel1Coupling(device.getChannel1().getCoupling()));
    initCommandList.push_back(cmdFactory.updateChannel2Coupling(device.getChannel2().getCoupling()));

    int i = 0;
    int total = initCommandList.size();

    for (Command *cmd : initCommandList)
    {
        float progress = ((float)i + 1.0f) / (float)total;
        ProgressHandler handler = progressHandler;

        Command::ResponseHandler rh = [=] {
            // handler(progress);
        };

        cmd->setResponseHandler(rh);
        sendCommand(cmd);
        i++;
    }
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

                    currentCommand->callResponseHandler();

                    // Remove current command
                    commandQueue.pop_front();
                    delete currentCommand;
                    currentCommand = NULL;
                    retries = 0;
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
                        currentCommand->callResponseHandler();

                        // Remove current command
                        commandQueue.pop_front();
                        delete currentCommand;
                        currentCommand = NULL;
                        retries = 0;
                    }
                }
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
        // Get current command from queue
        currentCommand = *(commandQueue.begin());

        // Transmit current command
        connector->transmit(currentCommand->getPayload(), 4);

        // Reset command limiting timout
        commandTimeout.reset();
    }
}

void Protocol::process()
{
    if (connector && connector->isConnected())
    {
        try
        {
            transmit();
            receive();
        }
        catch (ConnectException &e)
        {
            connectError = e.what();
        }
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