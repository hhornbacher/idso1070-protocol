#include "Protocol.h"

#include <iostream>

Protocol::Protocol()
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
    lock_guard<mutex> lock(commandMutex);
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
            progressHandlerRef(1.0f);
        stage2(finishedHandler);
    });
}

void Protocol::initStage2(Command::ResponseHandler finishedHandler)
{
    deque<Command *> cmds;

    {
        lock_guard<mutex> lock(deviceMutex);
        cmds.push_back(cmdFactory.updateSampleRate(device.getTimeBase(), device.getEnabledChannelsCount()));
        cmds.push_back(cmdFactory.updateFreqDivLowBytes((uint32_t)device.getTimeBase()));
        cmds.push_back(cmdFactory.updateFreqDivHighBytes((uint32_t)device.getTimeBase()));
        cmds.push_back(cmdFactory.updateChannelSelection(device.isChannelEnabled(CHANNEL_1), device.isChannelEnabled(CHANNEL_2)));
        cmds.push_back(cmdFactory.updateTriggerSourceAndSlope(device.getTriggerChannel(), device.getScopeMode(), device.getTriggerSlope()));
        cmds.push_back(cmdFactory.updateTriggerLevel(device.getTriggerLevel(), device.getTriggerTopPWM(), device.getTriggerBottomPWM()));
        cmds.push_back(cmdFactory.updatePreTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTriggerXPosition()));
        cmds.push_back(cmdFactory.updatePostTriggerLength(device.getSamplesNumberOfOneFrame(), device.getTriggerXPosition()));
        cmds.push_back(cmdFactory.readRamCount(device.getEnabledChannelsCount(), device.getSamplesNumberOfOneFrame(), device.getTriggerXPosition(), device.getPacketsNumber()));
        cmds.push_back(cmdFactory.updateRAMChannelSelection(device.isChannelEnabled(CHANNEL_1), device.isChannelEnabled(CHANNEL_2)));
        cmds.push_back(cmdFactory.updateChannelVolts125(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalDiv(CHANNEL_2)));
        cmds.push_back(cmdFactory.updateRelay1(device.getChannelVerticalDiv(CHANNEL_1)));
        cmds.push_back(cmdFactory.updateRelay2(device.getChannelVerticalDiv(CHANNEL_1)));
        cmds.push_back(cmdFactory.updateRelay3(device.getChannelVerticalDiv(CHANNEL_2)));
        cmds.push_back(cmdFactory.updateRelay4(device.getChannelVerticalDiv(CHANNEL_2)));
        cmds.push_back(cmdFactory.updateChannel1Level(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalPosition(CHANNEL_1), device.getChannelPWM(CHANNEL_1, (uint8_t)device.getChannelVerticalDiv(CHANNEL_1), 0), device.getChannelPWM(CHANNEL_1, (uint8_t)device.getChannelVerticalDiv(CHANNEL_1), 1)));
        cmds.push_back(cmdFactory.updateChannel2Level(device.getChannelVerticalDiv(CHANNEL_2), device.getChannelVerticalPosition(CHANNEL_2), device.getChannelPWM(CHANNEL_2, (uint8_t)device.getChannelVerticalDiv(CHANNEL_2), 0), device.getChannelPWM(CHANNEL_2, (uint8_t)device.getChannelVerticalDiv(CHANNEL_2), 1)));
        cmds.push_back(cmdFactory.updateChannelVolts125(device.getChannelVerticalDiv(CHANNEL_1), device.getChannelVerticalDiv(CHANNEL_2)));
        cmds.push_back(cmdFactory.updateTriggerMode(device.getCaptureMode(), device.getTriggerMode(), device.getScopeMode()));
        cmds.push_back(cmdFactory.updateTriggerLevel(device.getTriggerLevel(), device.getTriggerTopPWM(), device.getTriggerBottomPWM()));
        cmds.push_back(cmdFactory.updateChannel1Coupling(device.getChannelCoupling(CHANNEL_1)));
        cmds.push_back(cmdFactory.updateChannel2Coupling(device.getChannelCoupling(CHANNEL_2)));
    }

    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(cmds, [progressHandlerRef, finishedHandler] {
        if (progressHandlerRef)
            progressHandlerRef(1.0f);
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
    {
        lock_guard<mutex> lock(deviceMutex);
        cmds.push_back(cmdFactory.updateSampleRate(timeBase, device.getEnabledChannelsCount()));
        cmds.push_back(cmdFactory.updateFreqDivLowBytes((uint32_t)timeBase));
        cmds.push_back(cmdFactory.updateFreqDivHighBytes((uint32_t)timeBase));
    }
    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(cmds, [progressHandlerRef, responseHandler] {
        if (progressHandlerRef)
            progressHandlerRef(1.0f);
        responseHandler();
    });
}

void Protocol::setScopeMode(ScopeMode scopeMode, Command::ResponseHandler responseHandler)
{
    deque<Command *> cmds;
    {
        lock_guard<mutex> lock(deviceMutex);
        cmds.push_back(cmdFactory.updateTriggerSourceAndSlope(device.getTriggerChannel(), scopeMode, device.getTriggerSlope()));
        cmds.push_back(cmdFactory.updateTriggerMode(device.getCaptureMode(), device.getTriggerMode(), scopeMode));
    }
    ProgressHandler progressHandlerRef = progressHandler;
    sendCommandBatch(cmds, [progressHandlerRef, responseHandler] {
        if (progressHandlerRef)
            progressHandlerRef(1.0f);
        responseHandler();
    });
}

void Protocol::setCaptureMode(CaptureMode captureMode, Command::ResponseHandler responseHandler)
{
    lock_guard<mutex> lock(deviceMutex);
    Command *cmd = cmdFactory.updateTriggerMode(captureMode, device.getTriggerMode(), device.getScopeMode());
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
    lock_guard<mutex> lock(deviceMutex);
    Command *cmd = cmdFactory.updateTriggerMode(device.getCaptureMode(), mode, device.getScopeMode());
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::setTriggerChannel(TriggerChannel channel, Command::ResponseHandler responseHandler)
{
    lock_guard<mutex> lock(deviceMutex);
    Command *cmd = cmdFactory.updateTriggerSourceAndSlope(channel, device.getScopeMode(), device.getTriggerSlope());
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::setTriggerSlope(TriggerSlope slope, Command::ResponseHandler responseHandler)
{
    lock_guard<mutex> lock(deviceMutex);
    Command *cmd = cmdFactory.updateTriggerSourceAndSlope(device.getTriggerChannel(), device.getScopeMode(), slope);
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::setTriggerLevel(uint16_t level, Command::ResponseHandler responseHandler)
{
    lock_guard<mutex> lock(deviceMutex);
    Command *cmd = cmdFactory.updateTriggerLevel(level, device.getTriggerTopPWM(), device.getTriggerBottomPWM());
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::startSampling(Command::ResponseHandler responseHandler)
{
    lock_guard<mutex> lock(deviceMutex);
    Command *cmd = cmdFactory.startSampling();
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
}

void Protocol::stopSampling(Command::ResponseHandler responseHandler)
{
    Command *cmd = cmdFactory.readARMVersion();
    cmd->setResponseHandler(responseHandler);
    sendCommand(cmd);
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

        parse(currentResponse);
        if (currentCommand)
        {

            //     // If it's a wifi connector, we get two responses per command
            //     if (match && connector->getType() == CONNECTOR_WIFI)
            //         ignoreNextResponse = true;

            if (currentResponse->getCommandCode() == 0x04 && currentResponse->getCommandType() == TYPE_CONTROL)
            {
                lock_guard<mutex> lock(deviceMutex);
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
                {
                    lock_guard<mutex> lock(logMutex);
                    transmissionLog.push_back(new Transmission(*currentCommand, *currentResponse));
                }

                // Remove current command
                {
                    lock_guard<mutex> lock(commandMutex);
                    commandQueue.pop_front();
                }
                delete currentCommand;
                currentCommand = NULL;
                retries = 0;
            }
        }
        delete currentResponse;
        currentResponse = NULL;
    }
}

void Protocol::transmit()
{
    lock_guard<mutex> lock(commandMutex);
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

void Protocol::fetchSamples(SampleBuffer &buffer)
{
    lock_guard<mutex> lock(sampleMutex);
    if (sampleBuffer.channel1.size() > 0)
    {
        for (auto sample : sampleBuffer.channel1)
        {
            buffer.channel1.push_back(sample);
        }
        sampleBuffer.channel1.clear();
    }
    if (sampleBuffer.channel2.size() > 0)
    {
        for (auto sample : sampleBuffer.channel2)
        {
            buffer.channel2.push_back(sample);
        }
        sampleBuffer.channel2.clear();
    }
}

Connector *Protocol::getConnector()
{
    return connector;
}

void Protocol::fetchDevice(IDSO1070 &dev)
{
    lock_guard<mutex> lock(deviceMutex);
    dev = device;
}

void Protocol::fetchTransmissionLog(TransmissionLog &log)
{
    lock_guard<mutex> lock(logMutex);
    log = transmissionLog;
}

void Protocol::clearTransmissionLog(bool deleteObjects)
{
    lock_guard<mutex> lock(logMutex);
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
    lock_guard<mutex> lock(commandMutex);
    for (auto command : commandQueue)
    {
        delete command;
    }
    commandQueue.clear();
}

void Protocol::parse(Response *packet)
{
    switch (packet->getCommandType())
    {
    case TYPE_CONTROL:
        parseAAResponse(packet);
        return;
    case TYPE_EEPROM:
        parseEEResponse(packet);
        return;
    case TYPE_FPGA:
        parseFPGAResponse(packet);
        return;
    case TYPE_STATE:
        parseStateResponse(packet);
        return;
    default:
        return;
    }
}

void Protocol::parseAAResponse(Response *packet)
{
    switch (packet->getCommandCode())
    {
    case 0x02:
        // Parse FPGA version
        char version[9];
        for (int i = 0; i < 8; i++)
        {
            version[i] = 0x30 + packet->getPayload()[6 + i];
        }
        version[8] = 0;
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setFPGAFirmwareVersion(version);
        }
        return;
    case 0x04:
        // Parse sample
        parse((Sample *)packet);
        return;
    default:
        return;
    }
}

void Protocol::parseEEResponse(Response *packet)
{
    if (packet->getCommandCode() == 0xaa)
    {
        switch (packet->getHeader()[5])
        {
        case 0x00:
            parseEEPROMPage00(packet);
            return;
        case 0x04:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setFPGAAlert(packet->getPayload());
        }
            return;
        case 0x05:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setUserName((char *)packet->getPayload());
            device.setProductName((char *)&packet->getPayload()[12]);
        }
            return;
        case 0x07:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setDiffFixData(0, 0, packet->getPayload());
        }
            return;
        case 0x08:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setDiffFixData(0, 100, packet->getPayload());
        }
            return;
        case 0x09:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setDiffFixData(0, 200, packet->getPayload());
        }
            return;
        case 0x0a:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setDiffFixData(1, 0, packet->getPayload());
        }
            return;
        case 0x0b:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setDiffFixData(1, 100, packet->getPayload());
        }
            return;
        case 0x0c:
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setDiffFixData(1, 200, packet->getPayload());
        }
            return;
        default:
            return;
        }
    }
}

void Protocol::parseFPGAResponse(Response *packet)
{
    switch (packet->getCommandCode())
    {
    case CMDCODE_FORCE_TRIGGER:
        return;
    case CMDCODE_TRIGGER_MODE:
        parseTriggerMode(packet);
        return;
    case CMDCODE_SET_RELAY:
        parseRelay(packet);
        return;
    case CMDCODE_CHANNEL_SELECTION:
        return;
    case CMDCODE_TRIGGER_SOURCE:
        parseTriggerSourceAndSlope(packet);
        return;
    case CMDCODE_CHANNEL_VOLTS_DIV_125:
        parseVoltsDiv125(packet);
        return;
    case CMDCODE_PRE_TRIGGER_LENGTH:
        return;
    case CMDCODE_POST_TRIGGER_LENGTH:
        return;
    case CMDCODE_RAM_START_POSITION:
        return;
    case CMDCODE_RESERVED_DATA_OUTPUT:
        return;
    case CMDCODE_CH1_PWM:
        parseCh1ZeroLevel(packet);
        return;
    case CMDCODE_CH2_PWM:
        parseCh2ZeroLevel(packet);
        return;
    case CMDCODE_TRIGGER_PWM:
        parseTriggerLevel(packet);
        return;
    case CMDCODE_LOGIC_ANALYZER_1:
        return;
    case CMDCODE_LOGIC_ANALYZER_2:
        return;
    case CMDCODE_LOGIC_ANALYZER_3:
        return;
    case CMDCODE_SAMPLE_RATE:
        return;
    case CMDCODE_FREQ_DIV_LOW:
        parseFreqDivLowBytes(packet);
        return;
    case CMDCODE_FREQ_DIV_HIGH:
        parseFreqDivHighBytes(packet);
        return;
    case CMDCODE_SERIAL_BAUD_RATE:
        // Serial baud rate
        return;
    case CMDCODE_RAM_CHANNEL_SELECTION:
        parseRamChannelSelection(packet);
        return;
    case CMDCODE_READ_RAM_COUNT:
        return;
    default:
        return;
    }
}

void Protocol::parseStateResponse(Response *packet)
{
    switch (packet->getCommandCode())
    {
    // Battery level response
    case 0x03:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setBatteryLevel(packet->getPayload()[0]);
    }
        return;
    // Firmware version response
    case 0x04:
        char version[9];
        memcpy(version, packet->getPayload(), 8);
        version[8] = 0;
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setARMFirmwareVersion(version);
        }
        return;
    default:
        return;
    }
}

void Protocol::parseFreqDivLowBytes(Response *packet)
{
    int i = ((packet->getHeader()[6] & 255) << 8) | (packet->getHeader()[5] & 0xff);
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTimeBase((TimeBase)i);
    }
}

void Protocol::parseFreqDivHighBytes(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0xff) << 8) | (packet->getHeader()[5] & 0xff);
    {
        lock_guard<mutex> lock(deviceMutex);
        uint32_t update = (uint32_t)device.getTimeBase();
        update |= (i << 16);
        device.setTimeBase((TimeBase)update);
    }
}

void Protocol::parseRamChannelSelection(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x00:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.enableChannel(CHANNEL_1);
        device.enableChannel(CHANNEL_2);
    }
    break;
    case 0x01:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.disableChannel(CHANNEL_1);
        device.disableChannel(CHANNEL_2);
    }
    break;
    case 0x08:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.enableChannel(CHANNEL_1);
        device.disableChannel(CHANNEL_2);
        device.setSelectedChannel(CHANNEL_1);
    }
    break;
    case 0x09:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.disableChannel(CHANNEL_1);
        device.enableChannel(CHANNEL_2);
        device.setSelectedChannel(CHANNEL_2);
    }
    break;
    }
}

void Protocol::parseRelay(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0x80:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL1(CHANNEL_1, 1.0);
    }
    break;
    case 0xbf:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL1(CHANNEL_2, 0.1);
    }
    // device.getChannel2().setVoltageRL1(0.1);
    break;
    case 0xfb:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL2(CHANNEL_2, 0.1);
    }
    // device.getChannel2().setVoltageRL2(0.1);
    break;
    case 0xfd:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL2(CHANNEL_1, 0.1);
    }
    // device.getChannel1().setVoltageRL2(0.1);
    break;
    case 0x02:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL2(CHANNEL_1, 1.0);
    }
    // device.getChannel1().setVoltageRL2(1.0);
    break;
    case 0x04:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL2(CHANNEL_2, 1.0);
    }
    // device.getChannel2().setVoltageRL2(1.0);
    break;
    case 0x40:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL1(CHANNEL_2, 1.0);
    }
    // device.getChannel2().setVoltageRL1(1.0);
    break;
    case 0x7f:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltageRL1(CHANNEL_1, 0.1);
    }
    // device.getChannel1().setVoltageRL1(0.1);
    break;
    default:
        parseCoupling(packet);
        break;
    }
    // switch (packet->getHeader()[5])
    // {
    // case 0x80:
    // case 0x7f:
    // updateCh1VoltsDivStatusAfterReceivedRL1();
    //     return;
    // case 0xbf:
    // case 0x40:
    // updateCh2VoltsDivStatusAfterReceivedRL3();
    //     return;
    // case 0xfb:
    // case 0x04:
    // updateCh2VoltsDivStatusAfterReceivedRL4();
    //     return;
    // case 0xfd:
    // case 0x02:
    // updateCh1VoltsDivStatusAfterReceivedRL2();
    //     return;
    // default:
    //     return;
    // }
}

void Protocol::parseCh1ZeroLevel(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    {
        lock_guard<mutex> lock(deviceMutex);
        int ordinal = (int)device.getChannelVerticalDiv(CHANNEL_1);
        i = (int)roundf(mapValue(i, (float)device.getChannelPWM(CHANNEL_1, ordinal, 0), (float)device.getChannelPWM(CHANNEL_1, ordinal, 1), 8.0f, 248.0f));
        device.setChannelVerticalPosition(CHANNEL_1, i);
    }
}

void Protocol::parseCh2ZeroLevel(Response *packet)
{
    int i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    {
        lock_guard<mutex> lock(deviceMutex);
        int ordinal = (int)device.getChannelVerticalDiv(CHANNEL_2);
        i = (int)roundf(mapValue(i, (float)device.getChannelPWM(CHANNEL_2, ordinal, 0), (float)device.getChannelPWM(CHANNEL_2, ordinal, 1), 8.0f, 248.0f));
        device.setChannelVerticalPosition(CHANNEL_2, i);
    }
}

void Protocol::parseVoltsDiv125(Response *packet)
{
    switch (packet->getHeader()[5] & 3)
    {
    case 0:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltage125(CHANNEL_1, 1.0);
    }
    break;
    case 1:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltage125(CHANNEL_1, 2.0);
    }
    break;
    case 2:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltage125(CHANNEL_1, 5.0);
    }
    break;
    }
    // updateCh1VoltsDivStatusAfterReceived125();
    switch ((packet->getHeader()[5] >> 2) & 3)
    {
    case 0:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltage125(CHANNEL_2, 1.0);
    }
    break;
    case 1:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltage125(CHANNEL_2, 2.0);
    }
    break;
    case 2:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelVoltage125(CHANNEL_2, 5.0);
    }
    break;
    }
    // updateCh2VoltsDivStatusAfterReceived125();
}

void Protocol::parseTriggerLevel(Response *packet)
{
    uint16_t i = ((packet->getHeader()[6] & 0x0f) << 8) + (packet->getHeader()[5] & 0xff);
    {
        lock_guard<mutex> lock(deviceMutex);
        i = (uint16_t)roundf(mapValue(i, (float)device.getTriggerBottomPWM(), (float)device.getTriggerTopPWM(), 8.0f, 248.0f));
        device.setTriggerLevel(i);
    }
}

void Protocol::parseTriggerSourceAndSlope(Response *packet)
{
    uint8_t i = packet->getHeader()[5] & 3;

    if (i == 0)
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerChannel(TRIGCHAN_CH2);
    }
    else if (i == 1)
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerChannel(TRIGCHAN_CH1);
    }
    else if (i == 2)
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerChannel(TRIGCHAN_EXT);
    }
    if (packet->getHeader()[5] & (1 << 4))
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setScopeMode(SCOMODE_ANALOG);
    }
    else
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setScopeMode(SCOMODE_DIGITAL);
    }
    if (packet->getHeader()[5] & (1 << 7))
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerSlope(TRIGSLOPE_RISING);
    }
    else
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerSlope(TRIGSLOPE_FALLING);
    }
}

void Protocol::parseTriggerMode(Response *packet)
{
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setLittlePacketStatus(0);
    }

    uint8_t b = packet->getHeader()[5];
    if (b & (1 << 0))
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setCaptureMode(CAPMODE_ROLL);
    }
    else if (b & (1 << 3))
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setCaptureMode(CAPMODE_SCAN);
    }
    else
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setCaptureMode(CAPMODE_NORMAL);
    }
    if (b & (1 << 1))
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerMode(TRIGMODE_AUTO);
    }
    else if (b & (1 << 2))
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerMode(TRIGMODE_SINGLE);
    }
    else
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerMode(TRIGMODE_NORMAL);
    }
}

void Protocol::parseCoupling(Response *packet)
{
    switch (packet->getHeader()[5])
    {
    case 0xef:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelCoupling(CHANNEL_1, COUPLING_DC);
    }
    break;
    case 0xfe:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelCoupling(CHANNEL_2, COUPLING_DC);
    }
    break;
    case 0xff:
        if (packet->getHeader()[6] == 0x01)
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setChannelCoupling(CHANNEL_1, COUPLING_GND);
        }
        else if (packet->getHeader()[6] == 0x02)
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setChannelCoupling(CHANNEL_2, COUPLING_GND);
        }
        break;
    case 0x01:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelCoupling(CHANNEL_1, COUPLING_AC);
    }
    break;
    case 0x10:
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setChannelCoupling(CHANNEL_2, COUPLING_AC);
    }
    break;
    }
}

void Protocol::parseEEPROMPage00(Response *packet)
{
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setCaliLevel(packet->getPayload());
    }
    uint16_t *iArr;
    int i = 0;
    int i2 = 0;

    int tmpA = 0;
    int tmpB = 0;
    while (tmpB < 9)
    {
        i = tmpA + 1;
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setChannelPWM(CHANNEL_1, packet->getPayload()[tmpA] & 255, tmpB, 0);
            iArr = device.getChannelPWM(CHANNEL_1, tmpB);
        }
        int tmp = i + 1;
        iArr[0] = iArr[0] + ((packet->getPayload()[i] & 255) << 8);
        i = tmp + 1;
        {
            lock_guard<mutex> lock(deviceMutex);
            device.setChannelPWM(CHANNEL_1, packet->getPayload()[tmp] & 255, tmpB, 1);
            iArr = device.getChannelPWM(CHANNEL_1, tmpB);
        }
        i2 = i + 1;
        iArr[1] = ((packet->getPayload()[i] & 255) << 8) + iArr[1];
        tmpB++;
        tmpA = i2;
    }
    tmpB = tmpA;
    for (tmpA = 0; tmpA < 9; tmpA++)
    {
        lock_guard<mutex> lock(deviceMutex);
        i = tmpB + 1;
        device.setChannelPWM(CHANNEL_2, packet->getPayload()[tmpB] & 255, tmpA, 0);
        uint16_t *iArr2 = device.getChannelPWM(CHANNEL_2, tmpA);
        int tmp = i + 1;
        iArr2[0] = iArr2[0] + ((packet->getPayload()[i] & 255) << 8);
        i2 = tmp + 1;
        device.setChannelPWM(CHANNEL_2, packet->getPayload()[tmp] & 255, tmpA, 1);
        uint16_t *iArr3 = device.getChannelPWM(CHANNEL_2, tmpA);
        tmpB = i2 + 1;
        iArr3[1] = ((packet->getPayload()[i2] & 255) << 8) + iArr3[1];
    }
    i2 = tmpB + 1;
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerInnerPWM(0, packet->getPayload()[tmpB] & 255);
        iArr = device.getTriggerInnerPWM();
    }
    i = i2 + 1;
    iArr[0] = iArr[0] + ((packet->getPayload()[i2] & 255) << 8);
    tmpB = i + 1;
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerInnerPWM(1, packet->getPayload()[i] & 255);
        iArr = device.getTriggerInnerPWM();
    }
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerInnerPWM(0, packet->getPayload()[i] & 255);
        iArr = device.getTriggerInnerPWM();
    }
    i = tmpB + 1;
    iArr[0] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[0];
    tmpB = i + 1;
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerInnerPWM(1, packet->getPayload()[i] & 255);
        iArr = device.getTriggerInnerPWM();
    }
    i = tmpB + 1;
    iArr[1] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[1];
    tmpB = i + 1;
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerInnerPWM(2, packet->getPayload()[i] & 255);
        iArr = device.getTriggerInnerPWM();
    }
    i = tmpB + 1;
    iArr[2] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[2];
    tmpB = i + 1;
    {
        lock_guard<mutex> lock(deviceMutex);
        device.setTriggerInnerPWM(3, packet->getPayload()[i] & 255);
        iArr = device.getTriggerInnerPWM();
    }
    i = tmpB + 1;
    iArr[3] = ((packet->getPayload()[tmpB] & 255) << 8) + iArr[3];
    {
        lock_guard<mutex> lock(deviceMutex);
        if (device.getTriggerInnerPWM(2) < IDSO1070::SamplesCountPerPacket || device.getTriggerInnerPWM(2) > 4000)
        {
            device.setTriggerInnerPWM(2, device.getTriggerInnerPWM(0));
        }
        if (device.getTriggerInnerPWM(3) < IDSO1070::SamplesCountPerPacket || device.getTriggerInnerPWM(3) > 4000)
        {
            device.setTriggerInnerPWM(3, device.getTriggerInnerPWM(1));
        }
    }
    return;
}

void Protocol::parse(Sample *packet)
{
    uint8_t head = packet->getPayload()[0];
    if (head & (1 << 5))
    {
        int i = head & 0x0f;
        {
            lock_guard<mutex> lock(deviceMutex);
            if (device.getLittlePacketStatus() == i)
            {
                device.setLittlePacketStatus(device.getLittlePacketStatus() + 1);
                parseSamplePacket(packet, i);
                if (i == (device.getPacketsNumber() - 1))
                {
                    device.setLittlePacketStatus(0);

                    interpolateSamples();

                    //             if (this.connector.isSendingCommands()) {
                    //                 return;
                    //             }

                    if (head & (1 << 6))
                    {
                        printf("\n\nTrigger compared\n\n");
                        // trigger compared
                    }

                    if (head & (1 << 4))
                    {
                        printf("\n\nWave found\n\n");
                        // wave found
                    }
                    else
                    {
                        // wave not found
                    }
                }
                return;
            }
            device.setLittlePacketStatus(0);
        }
    }
}

void Protocol::parseSamplePacket(Sample *packet, int index)
{
    if (device.getEnabledChannelsCount() == 2)
    {
        lock_guard<mutex> lock(sampleMutex);
        parseBothChannelsData(packet, index);
    }
    else if (device.isChannelEnabled(CHANNEL_1) && !device.isChannelEnabled(CHANNEL_2))
    {
        lock_guard<mutex> lock(sampleMutex);
        parseChannel1Data(packet, index);
    }
    else if (device.isChannelEnabled(CHANNEL_2) && !device.isChannelEnabled(CHANNEL_1))
    {
        lock_guard<mutex> lock(sampleMutex);
        parseChannel2Data(packet, index);
    }
}

void Protocol::parseBothChannelsData(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * (IDSO1070::SamplesCountPerPacket / 2);

    while ((pos * 2) < IDSO1070::SamplesCountPerPacket)
    {

        if (device.getChannelCoupling(CHANNEL_2) == COUPLING_GND)
        {
            sampleBuffer.channel2.push_back(device.getChannelVerticalPosition(CHANNEL_2));
        }
        else
        {
            sampleBuffer.channel2.push_back((int8_t)(packet->getPayload()[(pos * 2) + 2] & 255));
            // cout << "Sample2: " << hex << ((uint16_t)(packet->getPayload()[(pos * 2) + 2] & 255)) << endl;
        }

        if (device.getChannelCoupling(CHANNEL_1) == COUPLING_GND)
        {
            sampleBuffer.channel1.push_back(device.getChannelVerticalPosition(CHANNEL_1));
        }
        else
        {
            sampleBuffer.channel1.push_back((int8_t)(packet->getPayload()[(pos * 2) + 1] & 255));
            cout << "Sample1: " << hex << ((uint16_t)(packet->getPayload()[(pos * 2) + 1] & 255)) << endl;

            cout << hexdump(packet->getHeader(), 509, 16) << endl;
        }

        //     statisticCh1Max(sampleOffset + pos, this.channel1.getSamples()[sampleOffset + pos]);
        //     statisticCh1Min(sampleOffset + pos, this.channel1.getSamples()[sampleOffset + pos]);
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void Protocol::parseChannel1Data(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * IDSO1070::SamplesCountPerPacket;
    while (pos < IDSO1070::SamplesCountPerPacket)
    {
        if (device.getChannelCoupling(CHANNEL_1) == COUPLING_GND)
        {
            sampleBuffer.channel1.push_back(device.getChannelVerticalPosition(CHANNEL_1));
        }
        else
        {
            sampleBuffer.channel1.push_back((int8_t)(packet->getPayload()[1 + pos] & 255));
        }
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void Protocol::parseChannel2Data(Sample *packet, int index)
{
    size_t pos = 0;

    size_t sampleOffset = index * IDSO1070::SamplesCountPerPacket;
    while (pos < IDSO1070::SamplesCountPerPacket)
    {
        if (device.getChannelCoupling(CHANNEL_2) == COUPLING_GND)
        {
            sampleBuffer.channel2.push_back(device.getChannelVerticalPosition(CHANNEL_2));
        }
        else
        {
            sampleBuffer.channel2.push_back((int8_t)(packet->getPayload()[1 + pos] & 255));
        }
        //     statisticCh2Max(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        //     statisticCh2Min(sampleOffset + pos, this.channel2.getSamples()[sampleOffset + pos]);
        pos++;
    }
}

void Protocol::interpolateSamples()
{
    int i = 1;
    // if (!(/* device.getTimeBase() == HDIV_1uS && */ device.getEnabledChannelsCount() == 2))
    // {
    //     i = 0;
    // }
    if (i != 0)
    {
        // IDSO1070Native.lerp_update(this.channel1.getInterpolatedSamples(), this.channel2.getInterpolatedSamples(), this.channel1.getSamples(), this.channel2.getSamples(), this.timeBase.ordinal(), channelStatus(), getTrigger().getTriggerChannel().ordinal(), (int)(getTrigger().getTriggerXPosition() * 100.0d), getTrigger().getTriggerLevel(), getTrigger().getTriggerSlope().ordinal(), IDSO1070::MemoryDepth, this.interpolateType.ordinal());
        if (device.isChannelEnabled(CHANNEL_1))
        {
            // System.arraycopy(this.channel1.getInterpolatedSamples(), 0, this.channel1.getSamples(), 0, IDSO1070::MemoryDepth);
        }
        if (device.isChannelEnabled(CHANNEL_2))
        {
            // System.arraycopy(this.channel2.getInterpolatedSamples(), 0, this.channel2.getSamples(), 0, IDSO1070::MemoryDepth);
        }
    }
}