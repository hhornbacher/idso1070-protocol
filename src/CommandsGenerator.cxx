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
    cmds.push_back(readEEROMPage(0x04));
    cmds.push_back(readEEROMPage(0x07));
    cmds.push_back(readEEROMPage(0x08));
    cmds.push_back(readEEROMPage(0x09));
    cmds.push_back(readEEROMPage(0x0a));
    cmds.push_back(readEEROMPage(0x0b));
    cmds.push_back(readEEROMPage(0x0c));
    return cmds;
}