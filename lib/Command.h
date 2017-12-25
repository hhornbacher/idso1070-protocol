#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>

#include <deque>

enum CommandCode
{
  FORCE_TRIGGER = 0x01,
  TRIGGER_MODE = 0x02,
  SET_RELAY = 0x03,
  CHANNEL_SELECTION = 0x04,
  TRIGGER_SOURCE = 0x05,
  CHANNEL_VOLTS_DIV_125 = 0x06,
  PRE_TRIGGER_LENGTH = 0x07,
  POST_TRIGGER_LENGTH = 0x08,
  RAM_START_POSITION = 0x09,
  RESERVED_DATA_OUTPUT = 0x0a,
  CH1_PWM = 0x0b,
  CH2_PWM = 0x0c,
  TRIGGER_PWM = 0x0d,
  LOGIC_ANALYZER_1 = 0x0e,
  LOGIC_ANALYZER_2 = 0x0f,
  LOGIC_ANALYZER_3 = 0x10,
  SAMPLE_RATE = 0x11,
  FREQ_DIV_LOW = 0x12,
  FREQ_DIV_HIGH = 0x13,
  SERIAL_BAUD_RATE = 0x14,
  RAM_CHANNEL_SELECTION = 0x15,
  READ_RAM_COUNT = 0x16
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