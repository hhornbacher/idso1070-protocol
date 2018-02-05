#include "Protocol.h"

Protocol::Protocol() : sampleBuffer1(SampleBufferSize), sampleBuffer2(SampleBufferSize), packetParser(device, sampleBuffer1, sampleBuffer2)
{
}

Protocol::~Protocol()
{
    disconnect();
    clearTransmissionLog();
    clearCommandQueue();
}

void Protocol::connect(string serialDevice)
{
    if (connector)
        disconnect();

    connector = new USBConnector(serialDevice);

    try
    {
        connector->start();
    }
    catch (Connector::Exception &e)
    {
        if (connectionLostHandler)
            connectionLostHandler(e);
    }
}

void Protocol::connect(string serverHost, int port)
{
    if (connector)
        disconnect();

    connector = new TCPConnector(serverHost, port);

    try
    {
        connector->start();
    }
    catch (Connector::Exception &e)
    {
        if (connectionLostHandler)
            connectionLostHandler(e);
    }
}

void Protocol::disconnect()
{
    if (connector)
    {
        clearCommandQueue();
        connector->stop();
        delete connector;
        connector = NULL;
    }
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

    // initLoadDataCmds.push_back(cmdFactory.readARMVersion());
    // initLoadDataCmds.push_back(cmdFactory.readFPGAVersion());
    initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x00));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x04));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x05));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x07));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x08));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x09));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x0a));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x0b));
    // initLoadDataCmds.push_back(cmdFactory.readEEPROMPage(0x0c));

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
    deque<Command *> cmds;
    cmds.push_back(cmdFactory.updateSampleRate(device.getDeviceTimeBase(), device.getEnabledChannelsCount()));
    cmds.push_back(cmdFactory.updateFreqDivLowBytes(device.getFreqDiv()));
    cmds.push_back(cmdFactory.updateFreqDivHighBytes(device.getFreqDiv()));
    cmds.push_back(cmdFactory.updateChannelSelection(device.isChannelEnabled(CHANNEL_1), device.isChannelEnabled(CHANNEL_2), device.isSampleRate200Mor250M()));
    cmds.push_back(cmdFactory.updateTriggerSourceAndSlope(device.getTriggerChannel(), device.getDeviceScopeMode(), device.getTriggerSlope()));
    cmds.push_back(cmdFactory.updateTriggerLevel(device.getTriggerLevel(), device.getTriggerTopPWM(), device.getTriggerBottomPWM()));
    cmds.push_back(cmdFactory.updatePreTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTriggerXPosition()));
    cmds.push_back(cmdFactory.updatePostTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTriggerXPosition()));
    cmds.push_back(cmdFactory.readRamCount(device.getEnabledChannelsCount(), device.getSamplesNumberOfOneFrame(), device.isSampleRate200Mor250M(), device.getTriggerXPosition(), device.getPacketsNumber()));
    cmds.push_back(cmdFactory.updateRAMChannelSelection(device.isChannelEnabled(CHANNEL_1), device.isChannelEnabled(CHANNEL_2)));
    cmds.push_back(cmdFactory.updateChannelVolts125(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalDiv(CHANNEL_2)));
    cmds.push_back(cmdFactory.updateRelay1(device.getChannelVerticalDiv(CHANNEL_1)));
    cmds.push_back(cmdFactory.updateRelay2(device.getChannelVerticalDiv(CHANNEL_1)));
    cmds.push_back(cmdFactory.updateRelay3(device.getChannelVerticalDiv(CHANNEL_2)));
    cmds.push_back(cmdFactory.updateRelay4(device.getChannelVerticalDiv(CHANNEL_2)));
    cmds.push_back(cmdFactory.updateChannel1Level(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalPosition(CHANNEL_1), device.getChannelPWM(CHANNEL_1, (uint8_t)device.getChannelVerticalDiv(CHANNEL_1), 0), device.getChannelPWM(CHANNEL_1, (uint8_t)device.getChannelVerticalDiv(CHANNEL_1), 1)));
    cmds.push_back(cmdFactory.updateChannel2Level(device.getChannelVerticalDiv(CHANNEL_2), device.getChannelVerticalPosition(CHANNEL_2), device.getChannelPWM(CHANNEL_2, (uint8_t)device.getChannelVerticalDiv(CHANNEL_2), 0), device.getChannelPWM(CHANNEL_2, (uint8_t)device.getChannelVerticalDiv(CHANNEL_2), 1)));
    cmds.push_back(cmdFactory.updateChannelVolts125(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalDiv(CHANNEL_2)));
    cmds.push_back(cmdFactory.updateTriggerMode(device.getDeviceCaptureMode(), device.getTriggerMode(), device.getDeviceScopeMode()));
    cmds.push_back(cmdFactory.updateTriggerLevel(device.getTriggerLevel(), device.getTriggerTopPWM(), device.getTriggerBottomPWM()));
    cmds.push_back(cmdFactory.updateChannel1Coupling(device.getChannelCoupling(CHANNEL_1)));
    cmds.push_back(cmdFactory.updateChannel2Coupling(device.getChannelCoupling(CHANNEL_2)));

    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(cmds, [progressHandlerRef, finishedHandler] {
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

void Protocol::setTimeBase(TimeBase timeBase, Command::ResponseHandler responseHandler)
{
    deque<Command *> cmds;
    cmds.push_back(cmdFactory.updateSampleRate(timeBase, device.getEnabledChannelsCount()));
    cmds.push_back(cmdFactory.updateFreqDivLowBytes(device.getDeviceTimeBaseFromFreqDiv()));
    cmds.push_back(cmdFactory.updateFreqDivHighBytes(device.getDeviceTimeBaseFromFreqDiv()));
    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(cmds, [progressHandlerRef, responseHandler] {
        if (progressHandlerRef)
            progressHandlerRef(100.0f);
        responseHandler();
    });
}

void Protocol::setScopeMode(ScopeMode scopeMode, Command::ResponseHandler responseHandler)
{
    deque<Command *> cmds;
    cmds.push_back(cmdFactory.updateTriggerSourceAndSlope(device.getTriggerChannel(), scopeMode, device.getTriggerSlope()));
    cmds.push_back(cmdFactory.updateTriggerMode(device.getDeviceCaptureMode(), device.getTriggerMode(), scopeMode));
    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(cmds, [progressHandlerRef, responseHandler] {
        if (progressHandlerRef)
            progressHandlerRef(100.0f);
        responseHandler();
    });
}

void Protocol::setCaptureMode(CaptureMode captureMode, Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.updateTriggerMode(captureMode, device.getTriggerMode(), device.getDeviceScopeMode());
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::enableChannel(ChannelSelector channel, Command::ResponseHandler responseHandler)
{
}

void Protocol::disableChannel(ChannelSelector channel, Command::ResponseHandler responseHandler)
{
}

void Protocol::setChannelVerticalDiv(ChannelSelector channel, VoltageDiv div, Command::ResponseHandler responseHandler)
{
}

void Protocol::setChannelCoupling(ChannelSelector channel, InputCoupling coupling, Command::ResponseHandler responseHandler)
{
}

void Protocol::setChannelVerticalPosition(ChannelSelector channel, uint16_t pos, Command::ResponseHandler responseHandler)
{
}

void Protocol::setTriggerMode(TriggerMode mode, Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.updateTriggerMode(device.getDeviceCaptureMode(), mode, device.getDeviceScopeMode());
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::setTriggerChannel(TriggerChannel channel, Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.updateTriggerSourceAndSlope(channel, device.getDeviceScopeMode(), device.getTriggerSlope());
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::setTriggerSlope(TriggerSlope slope, Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.updateTriggerSourceAndSlope(device.getTriggerChannel(), device.getDeviceScopeMode(), slope);
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::setTriggerLevel(uint16_t level, Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.updateTriggerLevel(level, device.getTriggerTopPWM(), device.getTriggerBottomPWM());
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

void Protocol::receive()
{
    connector->receive();
    while (connector->getResponseBufferSize() > 0)
    {
        // // This is only for some packets in wifi mode, drops current packet
        // if (ignoreNextResponse)
        // {
        //     currentResponse = connector->getLatestResponse();
        //     delete currentResponse;
        //     currentResponse = NULL;
        //     ignoreNextResponse = false;
        // }
        // // Default message handling
        // else
        // {
        currentResponse = connector->getLatestResponse();

        packetParser.parse(currentResponse);
        if (currentCommand)
        {
            // // Check for sample data packet
            // if (currentResponse->getCommandCode() == 0x04 && currentResponse->getCommandType() == TYPE_CONTROL)
            // {
            //     // Enable sampling mode
            //     sampling = true;

            //     // Create and parse sample
            //     Sample *sample = new Sample(currentResponse);
            //     packetParser.parse(sample);

            //     // Remove sample packet
            //     delete sample;

            //     currentCommand->callResponseHandler();

            //     // Remove current command
            //     commandQueue.pop_front();
            //     delete currentCommand;
            //     currentCommand = NULL;
            //     retries = 0;
            // }
            // else
            // {

            //     // If it's a wifi connector, we get two responses per command
            //     if (match && connector->getType() == CONNECTOR_WIFI)
            //         ignoreNextResponse = true;

            // if (currentCommand)
            // {

            if (currentResponse->getCommandCode() == 0x04 && currentResponse->getCommandType() == TYPE_CONTROL)
            {
                // Enable sampling flag
                device.setSampling(true);
            }

            // Check if received packet is response to sent command
            bool match = currentCommand->getPayload()[0] == currentResponse->getCommandType() &&
                         currentCommand->getPayload()[1] == currentResponse->getCommandCode();
            // Call handler of current command
            if (!match && retries < MaxCommandRetries)
                retries++;
            else
            {
                currentCommand->callResponseHandler();

                // Put transmission into log
                transmissionLog.push_back(new Transmission(*currentCommand, *currentResponse));

                // Remove current command
                commandQueue.pop_front();
                delete currentCommand;
                currentCommand = NULL;
                retries = 0;
            }
            // }
        }
        delete currentResponse;
        currentResponse = NULL;
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
        catch (Connector::Exception &e)
        {
            if (connectionLostHandler)
                connectionLostHandler(e);
        }
    }
}

void Protocol::setConnectionLostHandler(ConnectionLostHandler connectionLostHandler)
{
    this->connectionLostHandler = connectionLostHandler;
}

void Protocol::setProgressHandler(ProgressHandler progressHandler)
{
    this->progressHandler = progressHandler;
}

void Protocol::fetchChannelSamples(ChannelSelector channel, Sample::SampleBuffer &buffer)
{
    if (channel == CHANNEL_2)
    {
        buffer = sampleBuffer2;
    }
    buffer = sampleBuffer1;
}

Connector *Protocol::getConnector()
{
    return connector;
}

IDSO1070 &Protocol::getDevice()
{
    return device;
}

Protocol::TransmissionLog &Protocol::getTransmissionLog()
{
    return transmissionLog;
}

void Protocol::clearTransmissionLog(bool deleteObjects)
{
    if (deleteObjects)
    {
        for (auto transmission : transmissionLog)
        {
            delete transmission;
        }
    }
    transmissionLog.clear();
}

void Protocol::clearCommandQueue()
{
    for (auto command : commandQueue)
    {
        delete command;
    }
    commandQueue.clear();
}