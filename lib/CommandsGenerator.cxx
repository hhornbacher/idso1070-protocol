#include "CommandsGenerator.h"

Command *CommandsGenerator::readEEROMPage(uint8_t address)
{
    uint8_t cmdBuffer[4] = {0xee, 0xaa, address, 0x00};
    return new Command(cmdBuffer);
}

CommandQueue CommandsGenerator::readEEROM()
{
    CommandQueue cmds;
    cmds.push_back(readEEROMPage(0x00));
    cmds.push_back(readEEROMPage(0x01));
    cmds.push_back(readEEROMPage(0x04));
    cmds.push_back(readEEROMPage(0x05));
    cmds.push_back(readEEROMPage(0x06));
    cmds.push_back(readEEROMPage(0x07));
    cmds.push_back(readEEROMPage(0x08));
    cmds.push_back(readEEROMPage(0x09));
    cmds.push_back(readEEROMPage(0x0a));
    cmds.push_back(readEEROMPage(0x0b));
    cmds.push_back(readEEROMPage(0x0c));
    cmds.push_back(readEEROMPage(0x0f));
    return cmds;
}

Command *CommandsGenerator::readFPGAVersion()
{
    uint8_t cmdBuffer[4] = {0xaa, 0x02, 0x00, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandsGenerator::getBatteryLevel()
{
    uint8_t cmdBuffer[4] = {0x57, 0x03, 0x00, 0x00};
    return new Command(cmdBuffer);
}
Command *CommandsGenerator::getSampleRate()
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)SAMPLE_RATE, 0x00, 0x00};
    return new Command(cmdBuffer);
}
Command *CommandsGenerator::getFreqDivLowBytes()
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)FREQ_DIV_LOW, 0x00, 0x00};
    return new Command(cmdBuffer);
}
Command *CommandsGenerator::getFreqDivHighBytes()
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)FREQ_DIV_HIGH, 0x04, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandsGenerator::channelSelection()
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)CHANNEL_SELECTION, 0x02, 0x00};
    return new Command(cmdBuffer);
}

Command *CommandsGenerator::triggerSource()
{
    uint8_t cmdBuffer[4] = {0x55, (uint8_t)TRIGGER_SOURCE, 0x91, 0x00};
    return new Command(cmdBuffer);
}

CommandQueue CommandsGenerator::getTimebase()
{
    CommandQueue cmds;
    cmds.push_back(getSampleRate());
    cmds.push_back(getFreqDivLowBytes());
    cmds.push_back(getFreqDivHighBytes());
    return cmds;
}
