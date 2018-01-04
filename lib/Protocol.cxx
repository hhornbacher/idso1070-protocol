#include "Protocol.h"

Protocol::Protocol(Connector *connection) : cmdGen(&device), connection(connection),
                                            commandTimeout(200), parser(device)
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
    connection->start();
}

void Protocol::stop()
{
    connection->stop();
}

IDSO1070A &Protocol::getDevice()
{
    return device;
}

void Protocol::sendCommand(Command *cmd)
{
    commandQueue.add(cmd);
}

void Protocol::receive()
{
    connection->receive();
    if (connection->getPacketBufferLength() == IDSO1070A_PACKET_SIZE)
    {
        lastResponse = new Response(connection->getPacketBuffer());

        // Parse received packet
        bool success = parser.parse(lastResponse);

        // Call handler of current command
        commandQueue.getCurrent().callHandler(lastResponse->getPayload(), success);
        if (!success && retries < COMMAND_MAX_RETRIES)
        {
            retries++;
        }
        else
        {
            // Remove current command
            commandQueue.destroyCurrent();
            retries = 0;
        }

        expectedResponseCount--;
        connection->clearPacketBuffer();
        if (expectedResponseCount == 0)
            changeState(STATE_DONE);
    }
}

void Protocol::transmit()
{
    if (commandTimeout.isTimedOut())
    {
        connection->transmit(commandQueue.getCurrent().getPayload(), 4);
        if (commandQueue.getCurrent().getPayload()[0] == 0xee && !connection->isUsbConnection())
            expectedResponseCount = 2;
        expectedResponseCount = 1;
        // delete cmd;
        changeState(STATE_RESPONSE);
        commandTimeout.reset();
    }
}

Command *Protocol::getCommand(Commands cmd)
{
    switch (cmd)
    {
    case CMD_START_SAMPLING:
        return cmdGen.startSampling();
    case CMD_READ_EEROM_PAGE00:
        return cmdGen.readEEROMPage(0x00);
    case CMD_READ_EEROM_PAGE04:
        return cmdGen.readEEROMPage(0x04);
    case CMD_READ_EEROM_PAGE05:
        return cmdGen.readEEROMPage(0x05);
    case CMD_READ_EEROM_PAGE07:
        return cmdGen.readEEROMPage(0x07);
    case CMD_READ_EEROM_PAGE08:
        return cmdGen.readEEROMPage(0x08);
    case CMD_READ_EEROM_PAGE09:
        return cmdGen.readEEROMPage(0x09);
    case CMD_READ_EEROM_PAGE0A:
        return cmdGen.readEEROMPage(0x0a);
    case CMD_READ_EEROM_PAGE0B:
        return cmdGen.readEEROMPage(0x0b);
    case CMD_READ_EEROM_PAGE0C:
        return cmdGen.readEEROMPage(0x0c);
    case CMD_SELECT_CHANNEL:
        return cmdGen.selectChannel();
    case CMD_SELECT_RAM_CHANNEL:
        return cmdGen.selectRAMChannel();
    case CMD_READ_FPGA_VERSION:
        return cmdGen.readFPGAVersion();
    case CMD_READ_BATTERY_LEVEL:
        return cmdGen.readBatteryLevel();
    case CMD_READ_RAM_COUNT:
        return cmdGen.readRamCount();
    case CMD_GET_FREQDIV_LOW_BYTES:
        return cmdGen.getFreqDivLowBytes();
    case CMD_GET_FREQDIV_HIGH_BYTES:
        return cmdGen.getFreqDivHighBytes();
    case CMD_UPDATE_SAMPLE_RATE:
        return cmdGen.updateSampleRate();
    case CMD_UPDATE_TRIGGER_SOURCE_AND_SLOPE:
        return cmdGen.updateTriggerSourceAndSlope();
    case CMD_UPDATE_TRIGGER_LEVEL:
        return cmdGen.updateTriggerLevel();
    case CMD_UPDATE_CHANNEL_VOLTS125:
        return cmdGen.updateChannelVolts125();
    case CMD_UPDATE_TRIGGER_MODE:
        return cmdGen.updateTriggerMode();
    case CMD_RELAY1:
        return cmdGen.relay1();
    case CMD_RELAY2:
        return cmdGen.relay2();
    case CMD_RELAY3:
        return cmdGen.relay3();
    case CMD_RELAY4:
        return cmdGen.relay4();
    case CMD_CHANNEL1_LEVEL:
        return cmdGen.channel1Level();
    case CMD_CHANNEL2_LEVEL:
        return cmdGen.channel2Level();
    case CMD_CHANNEL1_COUPLING:
        return cmdGen.channel1Coupling();
    case CMD_CHANNEL2_COUPLING:
        return cmdGen.channel2Coupling();
    case CMD_PRE_TRIGGER:
        return cmdGen.preTrigger();
    case CMD_POST_TRIGGER:
        return cmdGen.postTrigger();
    }
}

void Protocol::changeState(States state)
{
    this->state = state;
}

void Protocol::process()
{
    switch (state)
    {
    case STATE_IDLE:
        if (commandQueue.getSize() > 0)
        {
            changeState(STATE_REQUEST);
        }
        break;
    case STATE_REQUEST:
        transmit();
        break;
    case STATE_RESPONSE:
        receive();
        break;
    case STATE_DONE:
        // if (responseHandler->onResponse(lastCommand, requestSuccess))
        changeState(STATE_IDLE);
        break;
    }
    // if (readBatteryTimeout.isTimedOut())
    // {
    //     printf("Send read battery!\n");
    //     sendCommands(cmdGen.readBatteryLevel());
    //     readBatteryTimeout.reset();
    // }
}

void Protocol::print()
{
    device.print();
}