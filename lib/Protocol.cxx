#include "Protocol.h"

Protocol::Protocol(Connector &connection) : connection(connection),
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

void Protocol::sendCommand(CommandGenerationFunction cmdFn)
{
    commandQueue.push_back(cmdFn);
}

void Protocol::sendCommands(std::deque<CommandGenerationFunction> cmdFns)
{
    for (auto cmdFn : cmdFns)
        commandQueue.push_back(cmdFn);
}

void Protocol::receive()
{
    connection.receive();
    if (connection.getPacketBufferLength() == IDSO1070A_PACKET_SIZE)
    {
        lastResponse = new Response(connection.getPacketBuffer());

        // Parse received packet
        bool success = parser.parse(lastResponse);

        // Call handler of current command
        if (!success && retries < COMMAND_MAX_RETRIES)
            retries++;
        else
        {
            currentCommand->callCommandHandler(lastResponse->getPayload(), success);
            // Remove current command generator function
            commandQueue.pop_front();
            retries = 0;
        }

        // Remove current command
        delete currentCommand;

        expectedResponseCount--;
        connection.clearPacketBuffer();
        if (expectedResponseCount == 0)
            changeState(STATE_IDLE);
    }
}

void Protocol::transmit()
{
    if (commandTimeout.isTimedOut())
    {
        CommandGenerationFunction generateCommand = *(commandQueue.begin());
        currentCommand = generateCommand();

        // Transmit current command
        connection.transmit(currentCommand->getPayload(), 4);

        // If command was readEEROM and it's a wifi connection, then we get two responses per command
        if (currentCommand->getPayload()[0] == TYPE_EE && !connection.isUsbConnection())
            expectedResponseCount = 2;
        expectedResponseCount = 1;

        changeState(STATE_RESPONSE);
        commandTimeout.reset();
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
        if (commandQueue.size() > 0)
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