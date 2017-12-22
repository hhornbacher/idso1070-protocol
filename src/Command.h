#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>

#include <deque>

enum CommandCode
{
    FORCE_TRIGGER = 1,
    TRIGGER_MODE = 2,
    SET_RELAY = 3,
    CHANNEL_SELECTION = 4,
    TRIGGER_SOURCE = 5,
    CHANNEL_VOLTS_DIV_125 = 6,
    PRE_TRIGGER_LENGTH = 7,
    POST_TRIGGER_LENGTH = 8,
    RAM_START_POSITION = 9,
    RESERVED_DATA_OUTPUT = 10,
    CH1_PWM = 11,
    CH2_PWM = 12,
    TRIGGER_PWM = 13,
    LOGIC_ANALYZER_1 = 14,
    LOGIC_ANALYZER_2 = 15,
    LOGIC_ANALYZER_3 = 16,
    SAMPLE_RATE = 17,
    FREQ_DIV_LOW = 18,
    FREQ_DIV_HIGH = 19,
    SERIAL_BAUD_RATE = 20,
    RAM_CHANNEL_SELECTION = 21,
    READ_RAM_COUNT = 22
};

class Command
{
  private:
    uint8_t payload[4];

  public:
    Command(CommandCode cmd);
    Command(uint8_t *cmd);

    uint8_t *getPayload();
};

typedef std::deque<Command *> CommandQueue;

#endif // _COMMAND_H_