#include "Protocol.h"

Protocol::Protocol() : packetParser(device, sampleBuffer1, sampleBuffer2)
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
        catch (ConnectionException &e)
        {
            if (connectionLostHandler)
                connectionLostHandler(e);
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
        catch (ConnectionException &e)
        {
            if (connectionLostHandler)
                connectionLostHandler(e);
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

void Protocol::sendCommandBatch(deque<Command *> cmds, Command::ResponseHandler finishedHandler)
{
    int i = 0;
    int total = cmds.size();

    for (Command *cmd : cmds)
    {
        if (cmd)
        {
            float progress = ((float)i + 1.0f) / (float)total;
            if ((i + 1) < total)
            {
                if (progressHandler)
                {
                    ProgressHandler progressHandlerRef = progressHandler;
                    cmd->setResponseHandler([progressHandlerRef, progress] {
                        progressHandlerRef(progress);
                    });
                }
            }
            else
            {
                cmd->setResponseHandler(finishedHandler);
            }
            sendCommand(cmd);
        }
        i++;
    }
}

void Protocol::init(Command::ResponseHandler finishedHandler)
{
    deque<Command *> initLoadDataCmds;

    initLoadDataCmds.push_back(cmdFactory.readARMVersion());
    initLoadDataCmds.push_back(cmdFactory.readFPGAVersion());
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x00));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x04));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x05));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x07));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x08));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x09));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x0a));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x0b));
    initLoadDataCmds.push_back(cmdFactory.readEEROMPage(0x0c));

    auto stage2 = bind(&Protocol::initStage2, this, placeholders::_1);

    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(initLoadDataCmds, [stage2, progressHandlerRef, finishedHandler] {
        if (progressHandlerRef)
            progressHandlerRef(100.0f);
        stage2(finishedHandler);
    });
}

void Protocol::initStage2(Command::ResponseHandler finishedHandler)
{
    deque<Command *> initDeviceCmds;
    initDeviceCmds.push_back(cmdFactory.updateSampleRate(device.getDeviceTimeBase(), device.getEnabledChannelsCount()));
    initDeviceCmds.push_back(cmdFactory.updateFreqDivLowBytes(device.getDeviceTimeBaseFromFreqDiv()));
    initDeviceCmds.push_back(cmdFactory.updateFreqDivHighBytes(device.getDeviceTimeBaseFromFreqDiv()));
    initDeviceCmds.push_back(cmdFactory.updateChannelSelection(device.isChannelEnabled(CHANNEL_1), device.isChannelEnabled(CHANNEL_2), device.isSampleRate200Mor250M()));
    initDeviceCmds.push_back(cmdFactory.updateTriggerSourceAndSlope(device.getTriggerChannel(), device.getDeviceScopeMode(), device.getTriggerSlope()));
    initDeviceCmds.push_back(cmdFactory.updateTriggerLevel(device.getTriggerLevel(), device.getTriggerTopPWM(), device.getTriggerBottomPWM()));
    initDeviceCmds.push_back(cmdFactory.updatePreTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTriggerXPosition()));
    initDeviceCmds.push_back(cmdFactory.updatePostTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTriggerXPosition()));
    initDeviceCmds.push_back(cmdFactory.readRamCount(device.getEnabledChannelsCount(), device.getSamplesNumberOfOneFrame(), device.isSampleRate200Mor250M(), device.getTriggerXPosition(), device.getPacketsNumber()));
    initDeviceCmds.push_back(cmdFactory.updateRAMChannelSelection(device.isChannelEnabled(CHANNEL_1), device.isChannelEnabled(CHANNEL_2)));
    initDeviceCmds.push_back(cmdFactory.updateChannelVolts125(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalDiv(CHANNEL_2)));
    initDeviceCmds.push_back(cmdFactory.updateRelay1(device.getChannelVerticalDiv(CHANNEL_1)));
    initDeviceCmds.push_back(cmdFactory.updateRelay2(device.getChannelVerticalDiv(CHANNEL_1)));
    initDeviceCmds.push_back(cmdFactory.updateRelay3(device.getChannelVerticalDiv(CHANNEL_2)));
    initDeviceCmds.push_back(cmdFactory.updateRelay4(device.getChannelVerticalDiv(CHANNEL_2)));
    initDeviceCmds.push_back(cmdFactory.updateChannel1Level(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalPosition(CHANNEL_1), device.getChannelPWM(CHANNEL_1, (uint8_t)device.getChannelVerticalDiv(CHANNEL_1), 0), device.getChannelPWM(CHANNEL_1, (uint8_t)device.getChannelVerticalDiv(CHANNEL_1), 1)));
    initDeviceCmds.push_back(cmdFactory.updateChannel2Level(device.getChannelVerticalDiv(CHANNEL_2), device.getChannelVerticalPosition(CHANNEL_2), device.getChannelPWM(CHANNEL_2, (uint8_t)device.getChannelVerticalDiv(CHANNEL_2), 0), device.getChannelPWM(CHANNEL_2, (uint8_t)device.getChannelVerticalDiv(CHANNEL_2), 1)));
    initDeviceCmds.push_back(cmdFactory.updateChannelVolts125(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalDiv(CHANNEL_2)));
    initDeviceCmds.push_back(cmdFactory.updateTriggerMode(device.getDeviceCaptureMode(), device.getTriggerMode(), device.getDeviceScopeMode()));
    initDeviceCmds.push_back(cmdFactory.updateTriggerLevel(device.getTriggerLevel(), device.getTriggerTopPWM(), device.getTriggerBottomPWM()));
    initDeviceCmds.push_back(cmdFactory.updateChannel1Coupling(device.getChannelCoupling(CHANNEL_1)));
    initDeviceCmds.push_back(cmdFactory.updateChannel2Coupling(device.getChannelCoupling(CHANNEL_2)));

    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(initDeviceCmds, [progressHandlerRef, finishedHandler] {
        if (progressHandlerRef)
            progressHandlerRef(100.0f);
        finishedHandler();
    });
}

void Protocol::readBatteryLevel(Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.readBatteryLevel();
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::startSampling(Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.startSampling();
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::stopSampling(Command::ResponseHandler responseHandler)
{
    // TODO: Find command to stop sampling...
}

void Protocol::setConnectionLostHandler(ConnectionLostHandler connectionLostHandler)
{
    this->connectionLostHandler = connectionLostHandler;
}

void Protocol::setProgressHandler(ProgressHandler progressHandler)
{
    this->progressHandler = progressHandler;
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
        // Default message handling
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
            // If we have no current command and sampling is enabled, this packet is probably a sample
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
    // Check if there are any commands
    if (commandQueue.size() > 0)
    {
        // Get current command from queue
        currentCommand = *(commandQueue.begin());

        // Transmit current command
        connector->transmit(currentCommand->getPayload(), 4);
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
        catch (ConnectionException &e)
        {
            if (connectionLostHandler)
                connectionLostHandler(e);
        }
    }
}

bool Protocol::isSampling()
{
    return sampling;
}

Connector *Protocol::getConnector()
{
    return connector;
}